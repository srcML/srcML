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

/*
  srcml_display_info.cpp display info about a given srcml input file.
*/

#include <srcml_display_metadata.hpp>
#include <src_prefix.hpp>
#include <srcml.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iomanip>

void srcml_aquire_unit_xml(srcml_archive* srcml_arch, int unit_index) {
    if (srcml_unit* unit = srcml_read_unit_position(srcml_arch, unit_index)) {
        std::cout << srcml_unit_get_xml(unit) << "\n";
        srcml_free_unit(unit);
    }
    // Problem getting the XML
}

void srcml_aquire_unit(srcml_archive* srcml_arch, int unit_index) {
    if (srcml_unit* unit = srcml_read_unit_position(srcml_arch, unit_index)) {
        char* unit_buffer = 0;
        int buffer_size = 0;
        srcml_unparse_unit_memory(unit, &unit_buffer, &buffer_size);
        std::cout << unit_buffer;
        srcml_free_unit(unit);
    }
    // Problem getting the unit
}

int srcml_unit_count(srcml_archive* srcml_arch) {
    int numUnits = 0;
    while (srcml_unit* unit = srcml_read_unit_header(srcml_arch)) {
        std::cerr << "test\n";
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

        std::cout << std::setw(5) << numUnits << ' ' << srcml_unit_get_filename(unit) << '\n';

        srcml_free_unit(unit);
    }
}

void srcml_display_info(srcml_archive* srcml_arch) {

    int numUnits = 0;

    while (true) {
        srcml_unit* unit = srcml_read_unit(srcml_arch);
        if (unit == 0)
            break;

        ++numUnits;

        /* Query options of srcml unit */
        const char* language = srcml_unit_get_language(unit);
        if (language)
            std::cout << "language=\"" << language << "\"\n";

        if (srcml_unit_get_filename(unit))
            std::cout << "filename=\"" << srcml_unit_get_filename(unit) << "\"\n";

        if (srcml_unit_get_directory(unit))
            std::cout << "directory=\"" << srcml_unit_get_directory(unit) << "\"\n";

        if (srcml_unit_get_version(unit))
            std::cout << "version=\"" << srcml_unit_get_version(unit) << "\"\n";

        srcml_free_unit(unit);
    }

    std::cout << "units=\"" << numUnits << "\"\n";
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
            const char* archive_info = srcml_archive_get_src_encoding(srcml_arch);
            if (archive_info)
                std::cout << "src-encoding=\"" << archive_info << "\"\n";
        }
        // srcml long info
        if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
            srcml_display_info(srcml_arch);
        }
        // srcml info
        if (srcml_request.command & SRCML_COMMAND_INFO) {
            srcml_display_info(srcml_arch);
        }
        // list filenames in srcml archive
        if (srcml_request.command & SRCML_COMMAND_LIST) {
            srcml_list_unit_files(srcml_arch);
        }
        // get specific unit xml
        if (srcml_request.unit > 0 && (srcml_request.command & SRCML_COMMAND_XML)) {
            srcml_aquire_unit_xml(srcml_arch, srcml_request.unit);
        }
        // get specific unit
        if (srcml_request.unit > 0 && !(srcml_request.command & SRCML_COMMAND_XML)) {
            srcml_aquire_unit(srcml_arch, srcml_request.unit);
        }
        // units
        if (srcml_request.command & SRCML_COMMAND_UNITS) {
            std::cout << srcml_unit_count(srcml_arch) << "\n";
        }

        srcml_close_archive(srcml_arch);
        srcml_free_archive(srcml_arch);
    }
}
