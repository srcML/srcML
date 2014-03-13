/**
 * @file srcml.cpp
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

#include <stdio.h>
#include <fcntl.h>
#include <srcml.h>
#include <srcml_cli.hpp>
#include <parse_queue.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_file.hpp>
#include <src_input_filesystem.hpp>
#include <src_input_stdin.hpp>
#include <srcml_input_srcml.hpp>
#include <src_output_libarchive.hpp>
#include <src_output_filesystem.hpp>
#include <srcml_display_info.hpp>
#include <srcml_list_unit_files.hpp>
#include <src_prefix.hpp>
#include <src_input_validator.hpp>
#include <src_language.hpp>
#include <trace_log.hpp>
#include <srcml_options.hpp>
#include <isxml.hpp>

#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <iostream>

#ifdef WIN32
#include <io.h>
#define  STDIN_FILENO   0       /* standard input file descriptor */
#define STDOUT_FILENO   1       /* standard output file descriptor */
#define STDERR_FILENO   2       /* standard error file descriptor */
#endif

int main(int argc, char * argv[]) {

    // parse the command line
    srcml_request_t srcml_request = parseCLI(argc, argv);

    SRCMLOptions::set(srcml_request.command);

    // version
    if (srcml_request.command & SRCML_COMMAND_VERSION) {
        std::cout <<  srcml_version_string() << "\n";
        std::cout << "srcml Version Number " << srcml_version_number() << "\n";
        std::cout << "libarchive Version " << ARCHIVE_VERSION_NUMBER << "\n";
        return 0;
    }

    const int SRCML_COMMAND_INSRCML =
        SRCML_COMMAND_LONGINFO |
        SRCML_COMMAND_INFO    |
        SRCML_COMMAND_INFO_FILENAME |
        SRCML_COMMAND_VERSION |
        SRCML_COMMAND_LIST |
        SRCML_COMMAND_UNITS |
        SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
        SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY |
        SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
        SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
        SRCML_COMMAND_DISPLAY_SRCML_ENCODING;

    // determine whether the input is xml(srcml) or not
    boost::optional<FILE*> fstdin;
    bool tosrc = false;
    bool tosrcml = false;
    bool insrcml = srcml_request.command & SRCML_COMMAND_INSRCML;
    if (srcml_request.command & SRCML_COMMAND_SRC) {
        tosrc = true;
    } else if (srcml_request.command & SRCML_COMMAND_SRCML) {
        tosrcml = true;
    } else if (!srcml_request.files_from.empty()) {
        tosrcml = true;
    } else {
        // find the first input that is not stdin
        boost::optional<std::string> nonstdin;
        BOOST_FOREACH(const std::string& input_filename, srcml_request.input) {
            if (input_filename != "-") {
                nonstdin = input_filename;
                break;
            }
        }

        if (nonstdin) {
            // base on extension
            tosrc = !boost::filesystem::path(nonstdin->c_str()).extension().compare(".xml");
        } else {
            // Note: If stdin only, then have to read from this FILE*, then make sure to use it below
            fstdin = fdopen(STDIN_FILENO, "r");

            // read the first 4 bytes as separate characters to get around byte ordering
            unsigned char data[4];
            ssize_t size = fread(&data, 1, 4, *fstdin);
            rewind(*fstdin);

            // pass the first 4 bytes and the size actually read in
            tosrc = isxml(data, size);
        }

        tosrcml = !tosrc;
    }

    if (tosrcml && (srcml_request.input.empty() || srcml_request.sawstdin) && !srcml_request.att_language) {
        std::cerr << "Using stdin requires a declared language\n";
        exit(1);
    }

    // src->srcml
    if (tosrcml) {


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
        srcml_write_open_filename(srcml_arch, srcml_request.output_filename->c_str());

        // gzip compression available from libsrcml
        if (srcml_request.output_filename->size() > 3 && srcml_request.output_filename->substr(srcml_request.output_filename->size() - 3) == ".gz")
            srcml_archive_enable_option(srcml_arch, SRCML_OPTION_COMPRESS);

        // setup the parsing queue
        ParseQueue queue(srcml_request.max_threads);

        // process command line inputs
        BOOST_FOREACH(const std::string& input_file, srcml_request.input) {

            // if stdin, then there has to be data
            if (!fstdin && (input_file == "-") && (srcml_request.command & SRCML_COMMAND_INTERACTIVE) && !src_input_stdin()) {
                return 1; // stdin was requested, but no data was received
            }

            std::string uri = src_prefix_add_uri(input_file);

            // split the URI
            std::string protocol;
            std::string resource;
            src_prefix_split_uri(uri, protocol, resource);

            std::string extension = boost::filesystem::extension(boost::filesystem::path(resource));

            // call handler based on prefix
            if (extension == ".xml") {
                srcml_input_srcml(resource, srcml_arch, fstdin);
            } else if ((protocol == "file") && is_directory(boost::filesystem::path(resource))) {
                src_input_filesystem(queue, srcml_arch, resource, srcml_request.att_language);

            } else if (protocol == "file" && extension != ".tar" && extension != ".gz" && extension != ".zip") {

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
    }

    if (insrcml) {
        // srcml->src language
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE){
        }
        // srcml->src directory
        else if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY){
        }
        // srcml->src filename
        else if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_FILENAME){
        }
        // srcml->src src version
        else if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION){
        }
        // srcml->src encoding
        else if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_ENCODING){
        }
        // srcml long info
        else if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
            srcml_display_info(srcml_request.input);
        }
        // srcml info
        else if (srcml_request.command & SRCML_COMMAND_INFO) {
            srcml_display_info(srcml_request.input);
        }
        // list filenames in srcml archive
        else if (srcml_request.command & SRCML_COMMAND_LIST) {
            srcml_list_unit_files(srcml_request.input);

        }
    }

    if (tosrc) {
        // srcml->src srcML file to filesystem
        if (srcml_request.command & SRCML_COMMAND_TO_DIRECTORY) {

            TraceLog log(std::cerr, *srcml_request.markup_options);

            // process command line inputs
            BOOST_FOREACH(const std::string& input_file, srcml_request.input) {

                srcml_archive* arch = srcml_create_archive();
                if (!fstdin)
                    srcml_read_open_filename(arch, input_file.c_str());
                else
                    srcml_read_open_FILE(arch, *fstdin);

                src_output_filesystem(arch, *srcml_request.output_filename, log);

                srcml_close_archive(arch);
                srcml_free_archive(arch);
            }

            // srcml->src extract individual unit in XML
        } else if (tosrc && (srcml_request.command & SRCML_COMMAND_XML) && srcml_request.unit != 0 && srcml_request.input.size() == 1) {

            srcml_archive* arch = srcml_create_archive();
            if (!fstdin)
                srcml_read_open_filename(arch, srcml_request.input[0].c_str());
            else
                srcml_read_open_FILE(arch, *fstdin);

            srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

            // TODO: We would have to use extend the API, or we will be creating/closing files
            srcml_archive* oarch = srcml_create_archive();
            srcml_write_open_filename(oarch, srcml_request.output_filename->c_str());

            srcml_write_unit(oarch, unit);

            srcml_close_archive(oarch);
            srcml_free_archive(oarch);

            srcml_close_archive(arch);
            srcml_free_archive(arch);

            // srcml->src extract individual unit to file
        } else if (tosrc && srcml_request.input.size() == 1 && srcml_request.unit > 0) {

            srcml_archive* arch = srcml_create_archive();
            if (!fstdin)
                srcml_read_open_filename(arch, srcml_request.input[0].c_str());
            else
                srcml_read_open_FILE(arch, *fstdin);

            srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

            if (*srcml_request.output_filename == "-")
                srcml_unparse_unit_fd(unit, STDOUT_FILENO);
            else
                srcml_unparse_unit_filename(unit, srcml_request.output_filename->c_str());

            srcml_close_archive(arch);
            srcml_free_archive(arch);

            // srcml->src srcML file extracted to stdout
        } else if (tosrc && srcml_request.input.size() == 1 && *srcml_request.output_filename == "-") {

            srcml_archive* arch = srcml_create_archive();
            if (!fstdin)
                srcml_read_open_filename(arch, srcml_request.input[0].c_str());
            else
                srcml_read_open_FILE(arch, *fstdin);

            srcml_unit* unit = srcml_read_unit(arch);

            srcml_unparse_unit_fd(unit, STDOUT_FILENO);

            srcml_close_archive(arch);
            srcml_free_archive(arch);

            // srcml->src srcML file to libarchive file
        } else if (tosrc) {

            // TODO: What if this is a simple, single file? or to stdout?
            archive* ar = archive_write_new();

            // setup compression and format
            // TODO: Needs to be generalized from output file extension
            archive_write_set_compression_gzip(ar);
            archive_write_set_format_pax_restricted(ar);

            archive_write_open_filename(ar, srcml_request.output_filename->c_str());

            // process command line inputs
            BOOST_FOREACH(const std::string& input_file, srcml_request.input) {

                srcml_archive* arch = srcml_create_archive();
                if (!fstdin)
                    srcml_read_open_filename(arch, input_file.c_str());
                else
                    srcml_read_open_FILE(arch, *fstdin);

                // extract this srcml archive to the source archive
                src_output_libarchive(arch, ar);

                srcml_close_archive(arch);
                srcml_free_archive(arch);
            }

            archive_write_close(ar);
            archive_write_finish(ar);

        } else {
            std::cerr << "Option not implemented" << '\n';
        }
    }

    srcml_cleanup_globals();

    return 0;
}
