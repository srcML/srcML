/**
 * @file srcml_display_info.cpp
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

#include <srcml_display_metadata.hpp>
#include <srcml_pretty.hpp>
#include <src_prefix.hpp>
#include <srcml.h>
#include <srcml_logger.hpp>
#include <srcml_options.hpp>
#include <iostream>
#include <iomanip>
#include <cstring>

namespace {

    const char* value(const char* call) {
        if (call)
            return call;
        else
            return "";
    }
};

// display all files in srcml archive
void srcml_list(srcml_archive* srcml_arch) {

    std::cout << "Source encoding: ";
    const char* src_encoding = srcml_archive_get_src_encoding(srcml_arch);
    std::cout << (src_encoding ? src_encoding : "(null)") << '\n';

    std::cout << "XML encoding: ";
    const char* xml_encoding = srcml_archive_get_xml_encoding(srcml_arch);
    std::cout << (xml_encoding ? xml_encoding : "(null)") << '\n'
              << "\n";

    int numUnits = 0;
    while (true) {
        srcml_unit* unit = srcml_archive_read_unit_header(srcml_arch);
        if (!unit)
            break;

        ++numUnits;
        std::cout << std::setw(5) << numUnits << " " 
                  << value(srcml_unit_get_filename(unit)) << '\t'
                  << value(srcml_unit_get_language(unit)) << '\t'
                  << value(srcml_unit_get_hash(unit)) << '\n';
        // TODO: Other parts of verbose here. Have to collect.
        srcml_unit_free(unit);
    }
    std::cout << "Total: " << numUnits << '\n';
}

void srcml_display_info(srcml_archive* srcml_arch, bool long_info) {

    auto nsSize = srcml_archive_get_namespace_size(srcml_arch);
    bool isarchive = srcml_archive_is_full_archive(srcml_arch);

    for (size_t i = 0; i < nsSize; ++i) {
        if (srcml_archive_get_namespace_uri(srcml_arch, i)) {
            auto prefix = srcml_archive_get_namespace_uri(srcml_arch, i);
            auto uri = srcml_archive_get_namespace_uri(srcml_arch, i);
            std::cout << "xmlns";
            if (prefix[0] != 0) {
                std::cout << prefix;
            }
            std::cout << "=\"" << uri << "\"\n";
        }
    }

    auto xml_encoding = srcml_archive_get_xml_encoding(srcml_arch);
    if (xml_encoding)
        std::cout << "encoding=" << "\"" << xml_encoding << "\"\n";

    srcml_unit* unit = srcml_archive_read_unit_header(srcml_arch);
    int unit_count = 0;

    if (!isarchive && unit) {
        auto language = srcml_unit_get_language(unit);
        if (language)
            std::cout << "language=" << "\"" << language << "\"\n";
    }

    auto url = srcml_archive_get_url(srcml_arch);
    if (url) {
        std::cout << "url=" << "\"" << url << "\"\n";
    }

    if (!isarchive && unit) {
        auto filename = srcml_unit_get_filename(unit);
        if (filename)
            std::cout << "filename=" << "\"" << filename << "\"\n";
    }

    if (long_info) {

        while (unit) {
            ++unit_count;

            srcml_unit_free(unit);
            unit = srcml_archive_read_unit_header(srcml_arch);
        }

        std::cout << "units=\"" << unit_count << "\"\n";

    } else if (unit) {

        srcml_unit_free(unit);
    }
}

int srcml_unit_count(srcml_archive* srcml_arch) {

    int numUnits = 0;
    while (true) {
        srcml_unit* unit = srcml_archive_read_unit_header(srcml_arch);

        if (!unit)
            break;

        if (srcml_unit_get_language(unit))
            ++numUnits;

        srcml_unit_free(unit);
    }
    return numUnits;
}

void srcml_display_metadata(const srcml_request_t& srcml_request, const srcml_input_t& src_input, const srcml_output_dest&) {

    int display_commands =  SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
                            SRCML_COMMAND_DISPLAY_SRCML_URL |
                            SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
                            SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
                            SRCML_COMMAND_DISPLAY_SRCML_ENCODING |
                            SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP |
                            SRCML_COMMAND_DISPLAY_SRCML_HASH;

    for (const auto& input : src_input) {
        // create the output srcml archive
        srcml_archive* srcml_arch = srcml_archive_create();

        if (contains<int>(input)) {
            if (srcml_archive_read_open_fd(srcml_arch, input) != SRCML_STATUS_OK) {
                SRCMLlog(CRITICAL_MSG, "srcml input cannot not be opened.");
                return;
            }
        }
        else if (contains<FILE*>(input)){
            if (srcml_archive_read_open_FILE(srcml_arch, input) != SRCML_STATUS_OK) {
                SRCMLlog(CRITICAL_MSG, "srcml input cannot not be opened.");
                return;
            }
        }
        else {
            if (srcml_archive_read_open_filename(srcml_arch, (src_prefix_resource(input).c_str())) != SRCML_STATUS_OK) {
                SRCMLlog(CRITICAL_MSG, "srcml input cannot not be opened.");
                return;
            }
        }

        // Overrides all others Perform a pretty output
        if (srcml_request.pretty_format) {
            srcml_pretty(srcml_arch, *srcml_request.pretty_format, srcml_request);
            return;
        }

        std::string pretty_meta_header;
        std::string pretty_meta_body;

        // HEADER ONLY METADATA
        if (option(SRCML_COMMAND_DISPLAY_SRCML_ENCODING)) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_ENCODING)
                pretty_meta_header += "%X\n";
            else
                pretty_meta_header += "encoding=\"%X\"\n";
        }

        if (option(SRCML_COMMAND_DISPLAY_SRCML_URL)) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_URL)
                pretty_meta_header += "%U\n";
            else
                pretty_meta_header += "url=\"%U\"\n";
        }

        // BODY METADATA
        if (option(SRCML_COMMAND_DISPLAY_SRCML_FILENAME)) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_FILENAME)
                pretty_meta_body += "%f\n";
            else
                pretty_meta_body += "filename=\"%f\"\n";
        }

        if (option(SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE)) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE)
                pretty_meta_body += "%l\n";
            else
                pretty_meta_body += "language=\"%l\"\n";
        }

        if (option(SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION)) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION)
                pretty_meta_body += "%v\n";
            else
                pretty_meta_body += "version=\"%v\"\n";
        }

        if (option(SRCML_COMMAND_DISPLAY_SRCML_HASH)) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_HASH)
                pretty_meta_body += "%h\n";
            else
                pretty_meta_body += "hash=\"%h\"\n";
        }

        if (option(SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP)) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP)
                pretty_meta_body += "%t\n";
            else
                pretty_meta_body += "timestamp=\"%t\"\n";
        }

        if (srcml_request.xmlns_prefix_query) {
            const char* prefix = srcml_archive_get_prefix_from_uri(srcml_arch, srcml_request.xmlns_prefix_query->c_str());
            if (prefix) {
                std::cout << prefix << '\n';
            }
        }

        if (pretty_meta_header != "" || pretty_meta_body != "") {
            srcml_pretty(srcml_arch, pretty_meta_header + " { " + pretty_meta_body + " } ", srcml_request);
        }

        // units
        if (option(SRCML_COMMAND_UNITS)) {
            std::cout << srcml_unit_count(srcml_arch) << "\n";
        }

        // srcml info
        if (option(SRCML_COMMAND_INFO)) {
            srcml_display_info(srcml_arch, false);
        }

        // srcml long info
        if (option(SRCML_COMMAND_LONGINFO)) {
            srcml_display_info(srcml_arch, true);
        }

        if (option(SRCML_COMMAND_LIST)) {
            srcml_list(srcml_arch);
        }

        srcml_archive_close(srcml_arch);
        srcml_archive_free(srcml_arch);
    }
}
