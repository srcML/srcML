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
#include <src_input_filesystem.hpp>
#include <src_input_stdin.hpp>
#include <srcml_display_info.hpp>
#include <srcml_list_unit_files.hpp>
#include <src_prefix.hpp>
#include <src_input_validator.hpp>
#include <src_language.hpp>

#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <iostream>

#ifdef WIN32
#include <io.h>
#endif

int main(int argc, char * argv[]) {

    // parse the command line
    srcml_request_t srcml_request = parseCLI(argc, argv);

    // version
    if (srcml_request.command & SRCML_COMMAND_VERSION) {
        std::cout <<  srcml_version_string() << "\n";
        std::cout << "srcml Version Number " << srcml_version_number() << "\n";
        std::cout << "libarchive Version " << ARCHIVE_VERSION_NUMBER << "\n";
        return 0;
    }

    // find the first input that is not stdin
    boost::optional<std::string> nonstdin;
    for (int i = 0; i < (int)srcml_request.positional_args.size(); ++i) {
        if (srcml_request.positional_args[i] != "-") {
            nonstdin = srcml_request.positional_args[i];
            break;
        }
    }

    // determine whether the input is xml(srcml) or not
    bool isxml = false;
    FILE* fstdin;
    char c = 0;
    bool isfstdin = false;
    if (nonstdin) {
        int firstinfd = open(nonstdin->c_str(), O_RDONLY);
        read(firstinfd, &c, 1);
        close(firstinfd);
    } else {
        // Note: If stdin only, then have to read from this file*
        isfstdin = true;
        fstdin = fdopen(STDIN_FILENO, "r");
        c = fgetc(fstdin);
        ungetc(c, fstdin);
    }
    isxml = c == '<';

    // src->srcml
    if (!isxml && (srcml_request.unit == 0) &&
        ((srcml_request.positional_args.size() > 1) ||
         (src_language(srcml_request.positional_args[0]).compare("xml") != 0))) {

        // create the output srcml archive
        srcml_archive* srcml_arch = srcml_create_archive();

        // set options for the output srcml archive
        if (srcml_request.att_encoding)
            srcml_archive_set_encoding(srcml_arch, srcml_request.att_encoding->c_str());

        if (srcml_request.att_filename && *srcml_request.att_filename != "-")
            srcml_archive_set_filename(srcml_arch, srcml_request.att_filename->c_str());

        if (srcml_request.att_directory)
            srcml_archive_set_directory(srcml_arch, srcml_request.att_directory->c_str());

        if (srcml_request.att_src_versions)
            srcml_archive_set_version(srcml_arch, srcml_request.att_src_versions->c_str());

        if (srcml_request.markup_options)
            srcml_archive_enable_option(srcml_arch, srcml_archive_get_options(srcml_arch) | *srcml_request.markup_options);

        // TODO: replace "" with boost::optional. NOTE: "" is not a valid language and should
        // be caught by srcml_cli.*
        if (srcml_request.att_language != "") 
            srcml_archive_set_language(srcml_arch, srcml_request.att_language.c_str());
        else 
            srcml_archive_set_language(srcml_arch, SRCML_LANGUAGE_NONE);

        srcml_archive_set_tabstop(srcml_arch, srcml_request.tabs);

        // archive or not
        if (srcml_request.positional_args.size() == 1 && !(*srcml_request.markup_options & SRCML_OPTION_ARCHIVE))
            srcml_archive_disable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
        else
            srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);

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
        srcml_write_open_filename(srcml_arch, srcml_request.output_filename.c_str());

        // gzip compression available from libsrcml
        if (srcml_request.output_filename.size() > 3 && srcml_request.output_filename.substr(srcml_request.output_filename.size() - 3) == ".gz")
            srcml_archive_enable_option(srcml_arch, SRCML_OPTION_COMPRESS);

        // setup the parsing queue
        ParseQueue queue(srcml_request.max_threads);

        // process command line inputs
        BOOST_FOREACH(const std::string& input_file, srcml_request.positional_args) {

            // if stdin, then there has to be data
            if (!isfstdin && (input_file == "-") && (srcml_request.command & SRCML_COMMAND_INTERACTIVE) && !src_input_stdin()) {
                return 1; // stdin was requested, but no data was received
            }

            std::string uri = src_prefix_add_uri(input_file);

            // split the URI
            std::string protocol;
            std::string resource;
            src_prefix_split_uri(uri, protocol, resource);

            // call handler based on prefix
            if (isfstdin) {
                src_input_libarchive(queue, srcml_arch, resource, srcml_request.att_language, true, fstdin);
            } else if ((protocol == "file") && is_directory(boost::filesystem::path(resource))) {
                src_input_filesystem(queue, srcml_arch, resource, srcml_request.att_language);
            } else if (protocol == "file") {
                src_input_libarchive(queue, srcml_arch, resource, srcml_request.att_language);
            } else if (protocol == "stdin") {
                src_input_libarchive(queue, srcml_arch, resource, srcml_request.att_language);
            }
        }

        // wait for the parsing queue to finish
        queue.wait();

        // close the created srcML archive
        srcml_close_archive(srcml_arch);
        srcml_free_archive(srcml_arch);
    }
    // srcml long info
    else if (isxml && srcml_request.command & SRCML_COMMAND_LONGINFO) {
        srcml_display_info(srcml_request.positional_args);
    }
    // srcml info
    else if (isxml && srcml_request.command & SRCML_COMMAND_INFO) {
        srcml_display_info(srcml_request.positional_args);
    }
    // list filenames in srcml archive
    else if (isxml && srcml_request.command & SRCML_COMMAND_LIST) {
        srcml_list_unit_files(srcml_request.positional_args);

    // srcml->src srcML file to filesystem
    } else if (isxml && (srcml_request.command & SRCML_COMMAND_TO_DIRECTORY) && srcml_request.positional_args.size() == 1) {

        srcml_archive* arch = srcml_create_archive();
        srcml_read_open_filename(arch, srcml_request.positional_args[0].c_str());

        // construct the relative directory
        std::string prefix;
        if (srcml_request.output_filename != "." && srcml_request.output_filename != "./")
            prefix = srcml_request.output_filename;
 
        int count = 0;
        while (srcml_unit* unit = srcml_read_unit_header(arch)) {

            // construct the relative directory
            boost::filesystem::path out(prefix);
            out /= srcml_unit_get_filename(unit);

            // create the path
            if (!is_directory(out.parent_path()))
                boost::filesystem::create_directories(out.parent_path());

            // unparse directory to filename
            srcml_unparse_unit_filename(unit, out.c_str());

            // trace
            ++count;
            std::cerr << std::setw(5) << count << ' ' << out.c_str() << '\n';
            
            srcml_free_unit(unit);
        }

        srcml_close_archive(arch);
        srcml_free_archive(arch);

    // srcml->src extract individual unit in XML
    } else if (isxml && (srcml_request.command & SRCML_COMMAND_XML) && srcml_request.unit != 0 && srcml_request.positional_args.size() == 1) {

        srcml_archive* arch = srcml_create_archive();
        srcml_read_open_filename(arch, srcml_request.positional_args[0].c_str());

        srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

        // TODO: We would have to use extend the API, or we will be creating/closing files
        srcml_archive* oarch = srcml_create_archive();
        srcml_write_open_filename(oarch, srcml_request.output_filename.c_str());

        srcml_write_unit(oarch, unit);

        srcml_close_archive(oarch);
        srcml_free_archive(oarch);

        srcml_close_archive(arch);
        srcml_free_archive(arch);

    // srcml->src extract individual unit to stdout
    } else if (isxml && srcml_request.unit != 0 && srcml_request.positional_args.size() == 1 && srcml_request.output_filename == "-") {

        srcml_archive* arch = srcml_create_archive();

        // we opened this already when checking for XML status
        srcml_read_open_FILE(arch, fstdin);

        srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

        srcml_unparse_unit_fd(unit, STDOUT_FILENO);

        srcml_close_archive(arch);
        srcml_free_archive(arch);

    // srcml->src extract individual unit to file
    } else if (isxml && srcml_request.unit != 0 && srcml_request.positional_args.size() == 1) {

        srcml_archive* arch = srcml_create_archive();
        srcml_read_open_filename(arch, srcml_request.positional_args[0].c_str());

        srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

        srcml_unparse_unit_filename(unit, srcml_request.output_filename.c_str());

        srcml_close_archive(arch);
        srcml_free_archive(arch);

    // srcml->src srcML file extracted to stdout
    } else if (isxml && srcml_request.positional_args.size() == 1 && srcml_request.output_filename == "-") {

        srcml_archive* arch = srcml_create_archive();
        srcml_read_open_FILE(arch, fstdin);

        srcml_unit* unit = srcml_read_unit(arch);

        srcml_unparse_unit_fd(unit, STDOUT_FILENO);

        srcml_close_archive(arch);
        srcml_free_archive(arch);

    // srcml->src srcML file to libarchive file
    } else if (isxml && srcml_request.positional_args.size() == 1) {

        // TODO: What if this is a simple, single file? or to stdout?
        archive* ar = archive_write_new();

        // setup compression and format
        // TODO: Needs to be generalized from output file extension
        archive_write_set_compression_gzip(ar);
        archive_write_set_format_pax_restricted(ar);

        archive_write_open_filename(ar, srcml_request.output_filename.c_str());

        srcml_archive* arch = srcml_create_archive();
        srcml_read_open_filename(arch, srcml_request.positional_args[0].c_str());

        while (srcml_unit* unit = srcml_read_unit(arch)) {

            // unparse the unit into its own buffer
            char* buffer;
            int buffer_size;
            srcml_unparse_unit_memory(unit, &buffer, &buffer_size);

            // setup the entry header
            archive_entry* entry = archive_entry_new();
            archive_entry_set_pathname(entry, srcml_unit_get_filename(unit));
            archive_entry_set_size(entry, buffer_size);
            archive_entry_set_filetype(entry, AE_IFREG);
            archive_entry_set_perm(entry, 0644);

            time_t now = time(NULL);
            archive_entry_set_atime(entry, now, 0);
            archive_entry_set_ctime(entry, now, 0);
            archive_entry_set_mtime(entry, now, 0);
            archive_write_header(ar, entry);

            // write the data to the entry
            archive_write_data(ar, buffer, buffer_size);

            // done with the archive entry
            archive_entry_free(entry);

            // done with the srcML unit
            srcml_free_unit(unit);
        }

        srcml_close_archive(arch);
        srcml_free_archive(arch);

        archive_write_close(ar);
        archive_write_finish(ar);

    } else {
        std::cerr << "Option not implemented" << '\n';
    }

    return 0;
}
