/*
  srcml.cpp

  Copyright (C) 2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  The srcml program to transform to/from the srcML format, plus provides a variety of
  querying and transformation features.

  Replaces the src2srcml and srcml2src of the original srcML toolkit.
*/

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

// code testing (temporary)
void libarchive2srcml(std::string filename);
void file2srcml(std::string filename);
void file2srcml_count(std::string filename);
void direct2srcml(std::string filename);

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

    // src->srcml
    if (srcml_request.positional_args.size() > 1 || src_language(srcml_request.positional_args[0]).compare("xml") != 0) {

        // create the output srcml archive
        srcml_archive* srcml_arch = srcml_create_archive();

        // set options for the output srcml archive
        if (srcml_request.encoding != "")
            srcml_archive_set_encoding(srcml_arch, srcml_request.encoding.c_str());

        if (srcml_request.filename)
            srcml_archive_set_filename(srcml_arch, srcml_request.filename->c_str());

        if (srcml_request.directory_set)
            srcml_archive_set_directory(srcml_arch, srcml_request.directory.c_str());

        if (srcml_request.src_versions_set)
            srcml_archive_set_version(srcml_arch, srcml_request.src_versions.c_str());

        if (srcml_request.markup_options != 0)
            srcml_archive_enable_option(srcml_arch, srcml_archive_get_options(srcml_arch) | srcml_request.markup_options);

        if (srcml_request.language != "") 
            srcml_archive_set_language(srcml_arch, srcml_request.language.c_str());
        else 
            srcml_archive_set_language(srcml_arch, SRCML_LANGUAGE_NONE);

        srcml_archive_set_tabstop(srcml_arch, srcml_request.tabs);

        // archive or not
        if (srcml_request.positional_args.size() == 1 && !(srcml_request.markup_options & SRCML_OPTION_ARCHIVE))
            srcml_archive_disable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
        else
            srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);

        // register file extensions
        BOOST_FOREACH(const std::string& ext, srcml_request.register_ext) {
            size_t pos = ext.find('=');
            srcml_archive_register_file_extension(srcml_arch, ext.substr(0, pos).c_str(), ext.substr(pos+1).c_str());
        }

        // register xml namespaces
        BOOST_FOREACH(const std::string& ns, srcml_request.xmlns_prefix) {
            size_t pos = ns.find('=');
            srcml_archive_register_namespace(srcml_arch, ns.substr(0,pos).c_str(), ns.substr(pos+1).c_str());
        }

        // create the srcML output file
        srcml_write_open_filename(srcml_arch, srcml_request.output.c_str());

        // setup the parsing queue
        ParseQueue queue(srcml_request.max_threads);

        // process command line inputs
        BOOST_FOREACH(const std::string& input_file, srcml_request.positional_args) {

            if (src_validate(input_file)) {
                // if stdin, then there has to be data
                if ((input_file == "-") && (srcml_request.command & SRCML_COMMAND_INTERACTIVE) && !src_input_stdin()) {
                    return 1; // stdin was requested, but no data was received
                }

                std::string uri = src_prefix_add_uri(input_file);

                // split the URI
                std::string protocol;
                std::string resource;
                src_prefix_split_uri(uri, protocol, resource);

                // call handler based on prefix
                if ((protocol == "file") && is_directory(boost::filesystem::path(resource))) {
                    src_input_filesystem(queue, srcml_arch, resource, srcml_request.language);
                } else if (protocol == "file") {
                    src_input_libarchive(queue, srcml_arch, resource, srcml_request.language);
                } else if (protocol == "stdin") {
                    src_input_libarchive(queue, srcml_arch, resource, srcml_request.language);
                }
            }
            else {
                // SETUP AN ERROR PARSE REQUEST FOR TRACING
                // This is temporary
                std::cerr << input_file << " is not accessible.\n";
            }
        }

        // wait for the parsing queue to finish
        queue.wait();

        // close the created srcML archive
        srcml_close_archive(srcml_arch);
        srcml_free_archive(srcml_arch);
    }
    // srcml long info
    else if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
        srcml_display_info(srcml_request.positional_args);
    }
    // srcml info
    else if (srcml_request.command & SRCML_COMMAND_INFO) {
        srcml_display_info(srcml_request.positional_args);
    }
    // list filenames in srcml archive
    else if (srcml_request.command & SRCML_COMMAND_LIST) {
        srcml_list_unit_files(srcml_request.positional_args);

    // srcml->src single file to stdout
    } else if (srcml_request.positional_args.size() == 1 && srcml_request.output == "-") {

        srcml_archive* arch = srcml_create_archive();
        srcml_read_open_filename(arch, srcml_request.positional_args[0].c_str());

        srcml_unit* unit = srcml_read_unit(arch);

        srcml_unparse_unit_fd(unit, STDOUT_FILENO);

        srcml_close_archive(arch);
        srcml_free_archive(arch);

    // srcml->src single file (non stdout)
    } else if (srcml_request.positional_args.size() == 1) {

        // TODO: What if this is a simple, single file? or to stdout?
        archive* ar = archive_write_new();

        // setup compression and format
        // TODO: Needs to be generalized from output file extension
        archive_write_set_compression_gzip(ar);
        archive_write_set_format_pax_restricted(ar);

        archive_write_open_filename(ar, srcml_request.output.c_str());

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
    }

    return 0;
}

// code testing (temporary)

#include <fcntl.h>
#include <archive.h>
#include <archive_entry.h>
#include <sys/types.h>
#include <unistd.h>
#include <boost/thread.hpp>

void read_from_pipe (int file) {

    // Parse srcml back to source (srcml2src)
    srcml_archive* arch = srcml_create_archive();
    srcml_read_open_fd(arch, file);
    srcml_unit* unit;

    while (true) {
        unit = srcml_read_unit(arch);

        if (unit == 0)
            break;

        srcml_unparse_unit_filename(unit, srcml_unit_get_filename(unit));
        srcml_free_unit(unit);
    }

    srcml_close_archive(arch);
    srcml_free_archive(arch);
}

void libarchive2srcml(std::string filename) {

    archive* arch = archive_read_new();
    archive_entry* arch_entry = archive_entry_new();
    archive_read_support_format_raw(arch);
    archive_read_support_compression_all(arch);


    filename = filename.substr(7);

    archive_read_open_filename(arch, filename.c_str(), 16384);
    archive_read_next_header(arch, &arch_entry);

    int datapipe[2];
    pipe(datapipe);

    boost::thread_group reader;
    reader.create_thread( boost::bind(read_from_pipe, datapipe[0]) );

    archive_read_data_into_fd(arch, datapipe[1]);

    close(datapipe[1]);

    archive_read_finish(arch);

    reader.join_all();
}

void file2srcml(std::string filename) {

    // Parse srcml back to source (srcml2src)
}

void file2srcml_header(std::string filename) {

    // Parse srcml back to source (srcml2src)
    srcml_archive* arch = srcml_create_archive();
    srcml_read_open_filename(arch, filename.c_str());
    srcml_unit* unit;

    while (true) {
        unit = srcml_read_unit_header(arch);
        if (unit == 0)
            break;

        srcml_unparse_unit_filename(unit, srcml_unit_get_filename(unit));
        srcml_free_unit(unit);
    }

    srcml_close_archive(arch);
    srcml_free_archive(arch);
}

void file2srcml_count(std::string filename) {

    //    filename = filename.substr(8);


    // Parse srcml back to source (srcml2src)
    srcml_archive* arch = srcml_create_archive();
    srcml_read_open_filename(arch, filename.c_str());

    int counter = 0;
    while (true) {
        srcml_unit* unit = srcml_read_unit_header(arch);
        if (unit == 0)
            break;

        ++counter;

        srcml_free_unit(unit);
    }

    fprintf(stderr, "DEBUG:  %s %s %d DATA: %d\n", __FILE__,  __FUNCTION__, __LINE__, counter);

    srcml_close_archive(arch);
    srcml_free_archive(arch);
}

void file2srcml_count_skip(std::string filename) {

    // Parse srcml back to source (srcml2src)
    srcml_archive* arch = srcml_create_archive();
    srcml_read_open_filename(arch, filename.c_str());

    int counter = 0;
    while (true) {
        srcml_unit* unit = srcml_read_unit_header(arch);
        if (unit == 0)
            break;

        ++counter;

        srcml_free_unit(unit);
    }

    fprintf(stderr, "DEBUG:  %s %s %d DATA: %d\n", __FILE__,  __FUNCTION__, __LINE__, counter);

    srcml_close_archive(arch);
    srcml_free_archive(arch);
}

void direct2srcml(std::string filename) {

    // Parse srcml back to source (srcml2src)
    srcml(filename.substr(7).c_str(), "foobar.cpp");
}
