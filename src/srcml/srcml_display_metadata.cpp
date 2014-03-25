/**
 * @file srcml_display_info.cpp
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

/*
  srcml_display_info.cpp display info about a given srcml input file.
*/

#include <srcml_display_metadata.hpp>
#include <src_prefix.hpp>
#include <srcml.h>
#include <srcml_list_unit_files.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

void srcml_display_info(const std::string& srcml_input) {

    int numUnits = 0;
    srcml_archive* srcml_arch = srcml_create_archive();
    srcml_read_open_filename(srcml_arch, srcml_input.c_str());

    while (true) {
        srcml_unit* unit = srcml_read_unit(srcml_arch);
        if (unit == 0)
            break;

        ++numUnits;

        /* Query options of srcml unit */
        const char* language = srcml_unit_get_language(unit);
        if (language)
            std::cout << "Language: " << language << "\n";

        if (srcml_unit_get_filename(unit))
            std::cout << "Filename: " << srcml_unit_get_filename(unit) << "\n";

        if (srcml_unit_get_directory(unit))
            std::cout << "Directory: " << srcml_unit_get_directory(unit) << "\n";

        if (srcml_unit_get_version(unit))
            std::cout << "Version: " << srcml_unit_get_version(unit) << "\n";

        srcml_free_unit(unit);
    }

    std::cout << "units=" << numUnits << "\n";

    srcml_free_archive(srcml_arch);
}

// TODO: Need to show encoding
// TODO: Need to not show language for archive
void srcml_display_info(const std::vector<std::string>& pos_args) {
    BOOST_FOREACH(const std::string& input_file, pos_args) {
        std::string resource;
        std::string protocol;
        src_prefix_split_uri(input_file, protocol, resource);
        boost::filesystem::path file (resource);
        if(file.extension().compare((const std::string &)std::string(".xml")) == 0)
            srcml_display_info(resource);
    }
}

void srcml_display_metadata(const srcml_request_t& srcml_request) {
    // create the output srcml archive
    srcml_archive* srcml_arch = srcml_create_archive();
    // Assuming one srcml input
    if (srcml_read_open_filename(srcml_arch, (src_prefix_resource(srcml_request.input[0]).c_str())) != SRCML_STATUS_OK) {
        std::cerr << "srcML file " << src_prefix_resource(srcml_request.input[0]) << " could not be opened.\n";
        return; // Error on opening the the srcml
    }

    // srcml->src language
    if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE){
        const char* archive_info = srcml_archive_get_language(srcml_arch);
        if (archive_info)
            std::cout << "Language: " << archive_info << "\n";
    }
    // srcml->src directory
    if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY){
        const char* archive_info = srcml_archive_get_directory(srcml_arch);
        if (archive_info)
            std::cout << "Directory: " << archive_info << "\n";
    }
    // srcml->src filename
    if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_FILENAME){
        const char* archive_info = srcml_archive_get_filename(srcml_arch);
        if (archive_info)
            std::cout << "Filename: " << archive_info << "\n";
    }
    // srcml->src src version
    if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION){
        const char* archive_info = srcml_archive_get_version(srcml_arch);
        if (archive_info)
            std::cout << "Version: " << archive_info << "\n";
    }
    // srcml->src encoding
    if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_ENCODING){
        const char* archive_info = srcml_archive_get_src_encoding(srcml_arch);
        if (archive_info)
            std::cout << "Source Encoding: " << archive_info << "\n";
    }
    // srcml long info
    if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
        srcml_display_info(srcml_request.input);
    }
    // srcml info
    if (srcml_request.command & SRCML_COMMAND_INFO) {
        srcml_display_info(srcml_request.input);
    }
    // list filenames in srcml archive
    if (srcml_request.command & SRCML_COMMAND_LIST) {
        srcml_list_unit_files(srcml_request.input);

    }
    
    srcml_free_archive(srcml_arch);
}
