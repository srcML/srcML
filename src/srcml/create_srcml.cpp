/**
 * @file create_srcml.hpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <create_srcml.hpp>
#include <srcml.h>
#include <boost/foreach.hpp>
#include <parse_queue.hpp>
#include <write_queue.hpp>
#include <srcml_consume.hpp>
#include <srcml_write.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_file.hpp>
#include <src_input_filesystem.hpp>
#include <src_input_filelist.hpp>
#include <src_input_stdin.hpp>
#include <src_input_text.hpp>
#include <src_prefix.hpp>
#include <srcml_input_srcml.hpp>
#include <trace_log.hpp>
#include <srcml_options.hpp>
#include <input_file.hpp>

void srcml_handler_dispatch(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          const srcml_input_src& input) {

    // call appropriate handler
    if (input.state == SRCML) {
        // libsrcml can apply gz decompression
        // all other srcml compressions require a per-input decompression stage
        srcml_input_src uninput = input;
        input_file(uninput);
        srcml_input_srcml(queue, srcml_arch, uninput);

    } else if (input.protocol == "text") {

        src_input_text(queue, srcml_arch, srcml_request, input.filename);

    } else if (input.protocol == "filelist") {

        src_input_filelist(queue, srcml_arch, srcml_request, input);

    } else if (input.protocol == "file" && input.isdirectory) {

        src_input_filesystem(queue, srcml_arch, srcml_request, input);

    } else if (input.protocol == "file" && input.archives.empty() && input.compressions.empty()) {
       
        src_input_file(queue, srcml_arch, srcml_request, input);

    } else {
       
        src_input_libarchive(queue, srcml_arch, srcml_request, input);
    }
}

// create srcml from the current request
void create_srcml(const srcml_request_t& srcml_request,
                  const srcml_input_t& input_sources,
                  const srcml_output_dest& destination) {

    // create the output srcml archive
    srcml_archive* srcml_arch = srcml_archive_create();

    // set options for the output srcml archive
    if (srcml_request.att_xml_encoding)
        srcml_archive_set_xml_encoding(srcml_arch, srcml_request.att_xml_encoding->c_str());

    if (srcml_request.src_encoding)
        srcml_archive_set_src_encoding(srcml_arch, srcml_request.src_encoding->c_str());

    // for single input src archives (e.g., .tar), filename attribute is the source filename (if not already given)
    if (srcml_request.att_url) {
        srcml_archive_set_url(srcml_arch, src_prefix_resource(*srcml_request.att_url).c_str());
    } else if (input_sources.size() == 1 && input_sources[0].archives.size() > 0) {

        // Cleanup filename
        std::string url_name = src_prefix_resource(input_sources[0].filename);
        while (url_name.at(0) == '.' || url_name.at(0) == '/') {
            url_name.erase(0,1);
        }
        
        srcml_archive_set_url(srcml_arch, url_name.c_str());
    }

    if (srcml_request.att_version)
        srcml_archive_set_version(srcml_arch, srcml_request.att_version->c_str());

    if (srcml_request.markup_options)
        srcml_archive_set_options(srcml_arch, srcml_archive_get_options(srcml_arch) | *srcml_request.markup_options);

    if (*srcml_request.markup_options & SRCML_OPTION_XML_DECL)
        srcml_archive_disable_option(srcml_arch, SRCML_OPTION_XML_DECL);
    else
        srcml_archive_enable_option(srcml_arch, SRCML_OPTION_XML_DECL);

    if (*srcml_request.markup_options & SRCML_OPTION_NAMESPACE_DECL)
        srcml_archive_disable_option(srcml_arch, SRCML_OPTION_NAMESPACE_DECL);
    else
        srcml_archive_enable_option(srcml_arch, SRCML_OPTION_NAMESPACE_DECL);

    if (srcml_request.att_language)
        srcml_archive_set_language(srcml_arch, srcml_request.att_language->c_str());
    else
        srcml_archive_set_language(srcml_arch, SRCML_LANGUAGE_NONE);

    srcml_archive_set_tabstop(srcml_arch, srcml_request.tabs);

    // non-archive when:
    //   only one input
    //   no cli request to make it an archive
    //   not a directory (if local file)
    // TODO: check if a plain file. Source archives, i.e., .tar.gz, always produce srcml archives
    if (input_sources.size() == 1 && input_sources[0].protocol != "filelist" &&
        !(srcml_request.markup_options && (*srcml_request.markup_options & SRCML_OPTION_ARCHIVE)) &&
        !input_sources[0].isdirectory && input_sources[0].archives.size() < 1) {

        srcml_archive_disable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
        
        // If --hash is used, force hash for single input
        if (*srcml_request.markup_options & SRCML_OPTION_HASH) {
            srcml_archive_enable_option(srcml_arch, SRCML_OPTION_HASH);
        }
        else {
            srcml_archive_disable_option(srcml_arch, SRCML_OPTION_HASH);
        }

    } else {

        // if this is an archive, then no filename attribute is allowed
        if (srcml_request.att_filename) {
            fprintf(stderr, "Attribute filename cannot be set for a srcML archive\n");
            exit(SRCML_STATUS_INVALID_ARGUMENT);
        }

        srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
        srcml_archive_enable_option(srcml_arch, SRCML_OPTION_HASH);
    }

    // register file extensions
    BOOST_FOREACH(const std::string& ext, srcml_request.language_ext) {
        size_t pos = ext.find('=');
        srcml_archive_register_file_extension(srcml_arch, ext.substr(0, pos).c_str(), ext.substr(pos+1).c_str());
    }

    // register xml namespaces
    std::map<std::string, std::string>::const_iterator itr;
    for(itr = srcml_request.xmlns_namespaces.begin(); itr != srcml_request.xmlns_namespaces.end(); ++itr){
        srcml_archive_register_namespace(srcml_arch, (*itr).first.c_str(), (*itr).second.c_str());
    }

    // create the srcML output file

    unsigned short compression = 0;
    if (destination.extension == ".gz")
        compression = 9;

    int status = 0;
    if (SRCML_COMMAND_NOARCHIVE & SRCMLOptions::get()) {

        /* when no archive, this one is just used as a clone, so just don't open it */

    } else if (contains<int>(destination)) {

        status = srcml_archive_write_open_fd(srcml_arch, *destination.fd);
    } else {

        status = srcml_archive_write_open_filename(srcml_arch, destination.c_str(), compression);
    }

    // gzip compression available directly from libsrcml

    // setup the parsing queue
    TraceLog log(SRCMLOptions::get());
    log.header();
    WriteQueue write_queue(boost::bind(srcml_write_request, _1, boost::ref(log)), srcml_request.command & SRCML_COMMAND_OUTPUT_ORDERED);
    ParseQueue parse_queue(srcml_request.max_threads, boost::bind(srcml_consume, _1, &write_queue), write_queue);

    // process input sources
    BOOST_FOREACH(const srcml_input_src& input, input_sources) {
/*
// if stdin, then there has to be data
// TODO: Safe to remove this? We already read data.
if (!contains<FILE*>(input) && (input.protocol == "stdin") && (srcml_request.command & SRCML_COMMAND_INTERACTIVE) &&
!src_input_stdin()) {
return; // stdin was requested, but no data was received
}
*/
        srcml_handler_dispatch(parse_queue, srcml_arch, srcml_request, input);
    }

    // wait for the parsing and writing queues to finish
    parse_queue.wait();
    write_queue.wait();

    log.report();
    
    // close the created srcML archive
    srcml_archive_close(srcml_arch);
    srcml_archive_free(srcml_arch);

    // if we were writing to a file descriptor, then close it
    if (contains<int>(destination))
        close(*destination.fd);
}
