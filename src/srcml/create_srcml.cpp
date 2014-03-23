/**
 * @file create_srcml.hpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <create_srcml.hpp>
#include <stdio.h>
#include <fcntl.h>
#include <srcml.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <src_prefix.hpp>
#include <parse_queue.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_file.hpp>
#include <src_input_filesystem.hpp>
#include <src_input_stdin.hpp>
#include <srcml_input_srcml.hpp>

// create srcml from the current request
void create_srcml(srcml_input_t& input_sources,
                  srcml_request_t& srcml_request,
                  boost::optional<FILE*> fstdin,
                  boost::optional<int> fdout) {

    // create the output srcml archive
    srcml_archive* srcml_arch = srcml_create_archive();

    // set options for the output srcml archive
    if (srcml_request.att_xml_encoding)
        srcml_archive_set_encoding(srcml_arch, srcml_request.att_xml_encoding->c_str());

    if (srcml_request.src_encoding)
        srcml_archive_set_src_encoding(srcml_arch, srcml_request.src_encoding->c_str());

    if (srcml_request.att_filename)
        srcml_archive_set_filename(srcml_arch, srcml_request.att_filename->c_str());

    if (srcml_request.att_directory)
        srcml_archive_set_directory(srcml_arch, srcml_request.att_directory->c_str());

    if (srcml_request.att_version)
        srcml_archive_set_version(srcml_arch, srcml_request.att_version->c_str());

    if (srcml_request.markup_options)
        srcml_archive_set_options(srcml_arch, srcml_archive_get_options(srcml_arch) | *srcml_request.markup_options);

    if (srcml_request.att_language)
        srcml_archive_set_language(srcml_arch, srcml_request.att_language->c_str());
    else
        srcml_archive_set_language(srcml_arch, SRCML_LANGUAGE_NONE);

    srcml_archive_set_tabstop(srcml_arch, srcml_request.tabs);

    // non-archive when:
    //   only one input
    //   not a directory (if local file)
    //   no cli request to make it a directory
    if (srcml_request.input.size() == 1 &&
        !is_directory(boost::filesystem::path(src_prefix_resource(srcml_request.input[0]))) &&
        !(srcml_request.markup_options &&
          (*srcml_request.markup_options & SRCML_OPTION_ARCHIVE))) {

        srcml_archive_disable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
    } else {
        srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
    }

    srcml_archive_disable_option(srcml_arch, SRCML_OPTION_HASH);
    srcml_archive_disable_option(srcml_arch, SRCML_OPTION_TIMESTAMP);

    // register file extensions
    BOOST_FOREACH(const std::string& ext, srcml_request.language_ext) {
        size_t pos = ext.find('=');
        srcml_archive_register_file_extension(srcml_arch, ext.substr(0, pos).c_str(), ext.substr(pos+1).c_str());
    }

    // register xml namespaces
    BOOST_FOREACH(const std::string& ns, srcml_request.xmlns_prefix) {
        size_t pos = ns.find('=');
        srcml_archive_register_namespace(srcml_arch, ns.substr(0,pos).c_str(), ns.substr(pos+1).c_str());
    }

    // create the srcML output file. if compressed, must go through libarchive thread
    int status = 0;
    if (!fdout) {
        status = srcml_write_open_filename(srcml_arch, srcml_request.output_filename->c_str());
    } else {
        status = srcml_write_open_fd(srcml_arch, *fdout);
    }

    // gzip compression available from libsrcml
    if (srcml_request.output_filename->size() > 3 && srcml_request.output_filename->substr(srcml_request.output_filename->size() - 3) == ".gz")
        srcml_archive_enable_option(srcml_arch, SRCML_OPTION_COMPRESS);

    // setup the parsing queue
    ParseQueue queue(srcml_request.max_threads);

    // process command line inputs
    BOOST_FOREACH(const std::string& input_file, srcml_request.input) {

        // if stdin, then there has to be data
        if (!fstdin && (input_file == "-") && (srcml_request.command & SRCML_COMMAND_INTERACTIVE) && !src_input_stdin()) {
            return; // stdin was requested, but no data was received
        }

        std::string uri = src_prefix_add_uri(input_file);

        // split the URI
        std::string protocol;
        std::string resource;
        src_prefix_split_uri(uri, protocol, resource);

        std::string extension = boost::filesystem::extension(boost::filesystem::path(resource));

        // call handler based on prefix
        if (!fstdin && extension == ".xml") {

            srcml_input_srcml(resource, srcml_arch, fstdin);

        } else if (!fstdin && (protocol == "file") && is_directory(boost::filesystem::path(resource))) {

            src_input_filesystem(queue, srcml_arch, resource, srcml_request.att_language);

        } else if (!fstdin && protocol == "file" && !(is_archive(extension)) && !(is_compressed(extension))) {

            src_input_file(queue, srcml_arch, resource, srcml_request.att_language, srcml_request.att_filename, srcml_request.att_directory, srcml_request.att_version);

        } else {

            src_input_libarchive(queue, srcml_arch, uri, srcml_request.att_language, srcml_request.att_filename, srcml_request.att_directory, srcml_request.att_version, fstdin);

        }
    }

    // wait for the parsing queue to finish
    queue.wait();

    // close the created srcML archive
    srcml_close_archive(srcml_arch);
    srcml_free_archive(srcml_arch);

    // if we were writing to a file descriptor, then close it
    if (fdout)
        close(*fdout);
}
