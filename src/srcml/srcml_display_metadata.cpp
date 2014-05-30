/**
 * @file srcml_display_info.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#include <srcml_display_metadata.hpp>
#include <src_prefix.hpp>
#include <srcml.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iomanip>

// display unit names and total count
int srcml_unit_count(srcml_archive* srcml_arch) {
    int numUnits = 0;
    while (srcml_unit* unit = srcml_read_unit_header(srcml_arch)) {
        std::cerr << srcml_unit_get_filename(unit) << '\n';
        ++numUnits;
        srcml_free_unit(unit);
    }
    return numUnits;   
}

// display all files in srcml archive
void srcml_list_unit_files(srcml_archive* srcml_arch) {

    int numUnits = 0;
    while (srcml_unit* unit = srcml_read_unit_header(srcml_arch)) {
        ++numUnits;
        std::cout << numUnits << '\t' << std::setw(5) << srcml_unit_get_filename(unit) << '\n';
        srcml_free_unit(unit);
    }
}

void srcml_display_info(srcml_archive* srcml_arch) {

    if(srcml_archive_get_encoding(srcml_arch))
        std::cout << "encoding=" << "\"" << srcml_archive_get_encoding(srcml_arch) << "\"\n";
    if(srcml_archive_get_namespace_uri(srcml_arch, 0))
        std::cout << "xmlns=" << "\"" << srcml_archive_get_namespace_uri(srcml_arch, 0) << "\"\n";

}

void srcml_display_unit_count(srcml_archive* srcml_arch) {
    int num_units = 0;
    while (srcml_unit* unit = srcml_read_unit_header(srcml_arch)) {
        ++num_units;
        srcml_free_unit(unit);
    }
    std::cout << "units=" << "\"" << num_units << "\"\n";
}

void srcml_display_metadata(const srcml_request_t& srcml_request, const srcml_input_t& src_input, const srcml_output_dest&) {
    int display_commands = SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
                            SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
                            SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY |
                            SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
                            SRCML_COMMAND_DISPLAY_SRCML_ENCODING;

    BOOST_FOREACH(const srcml_input_src& input, src_input) {
        // create the output srcml archive
        srcml_archive* srcml_arch = srcml_create_archive();

        if (contains<int>(input)) {
            if (srcml_read_open_fd(srcml_arch, input) != SRCML_STATUS_OK) {
                std::cerr << "Srcml input cannot not be opened.\n";
                return;
            }
        }
        else if (contains<FILE*>(input)){
            if (srcml_read_open_FILE(srcml_arch, input) != SRCML_STATUS_OK) {
                std::cerr << "Srcml input cannot not be opened.\n";
                return;
            }   
        }
        else {
            if (srcml_read_open_filename(srcml_arch, (src_prefix_resource(input).c_str())) != SRCML_STATUS_OK) {
                std::cerr << "Srcml input cannot not be opened.\n";
                return;
            }
        }

        // srcml->src language
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE){
            const char* archive_info = srcml_archive_get_language(srcml_arch);
            if (archive_info) {
                if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE)
                    std::cout << archive_info << "\n";
                else
                    std::cout << "language=\"" << archive_info << "\"\n";
            }
        }
        // srcml->src filename
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_FILENAME){
            const char* archive_info = srcml_archive_get_filename(srcml_arch);
            if (archive_info) {
                if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_FILENAME)
                    std::cout << archive_info << "\n";
                else
                    std::cout << "filename=\"" << archive_info << "\"\n";
            }
        }
        // srcml->src directory
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY){
            const char* archive_info = srcml_archive_get_directory(srcml_arch);
            if (archive_info) {
                if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY)
                    std::cout << archive_info << "\n";
                else
                    std::cout << "directory=\"" << archive_info << "\"\n";
            }
        }
        // srcml->src src version
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION){
            const char* archive_info = srcml_archive_get_version(srcml_arch);
            if (archive_info) {
                if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION)
                    std::cout << archive_info << "\n";
                else
                    std::cout << "version=\"" << archive_info << "\"\n";
            }
        }
        // srcml->src encoding
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_ENCODING){
            const char* archive_info = srcml_archive_get_encoding(srcml_arch);

            if (archive_info)
                std::cout << "encoding=\"" << archive_info << "\"\n";
        }
        // srcml long info
        if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
            srcml_display_info(srcml_arch);
            srcml_display_unit_count(srcml_arch);
        }
        // srcml info
        if (srcml_request.command & SRCML_COMMAND_INFO) {
            srcml_display_info(srcml_arch);
        }
        // list filenames in srcml archive
        if (srcml_request.command & SRCML_COMMAND_LIST) {
            srcml_list_unit_files(srcml_arch);
        }
        // units
        if (srcml_request.command & SRCML_COMMAND_UNITS) {
            std::cout << srcml_unit_count(srcml_arch) << "\n";
        }

        srcml_close_archive(srcml_arch);
        srcml_free_archive(srcml_arch);
    }
}
