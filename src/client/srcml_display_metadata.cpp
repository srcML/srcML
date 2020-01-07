/**
 * @file srcml_display_info.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <srcml_display_metadata.hpp>
#include <srcml_pretty.hpp>
#include <src_prefix.hpp>
#include <srcml.h>
#include <SRCMLStatus.hpp>
#include <srcml_options.hpp>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <srcml_utilities.hpp>

namespace {

    const char* value(const char* call) {
        return call ? call : "";
    }

    // display all files in srcml archive
    void srcml_list(srcml_archive* srcml_arch) {

        std::cout << "XML encoding: ";
        const char* xml_encoding = srcml_archive_get_xml_encoding(srcml_arch);
        std::cout << (xml_encoding ? xml_encoding : "(null)") << '\n';

        int numUnits = 0;
        long LOC = 0;
        while (true) {
            std::unique_ptr<srcml_unit> unit(srcml_archive_read_unit(srcml_arch));
            if (!unit)
                break;

            ++numUnits;
            std::cout << std::setw(5) << numUnits;
            std::cout << std::setw(5) << std::right << value(srcml_unit_get_language(unit.get())) ;
            std::cout << ' ' << std::setw(5) << std::right << srcml_unit_get_loc(unit.get());
            std::cout << ' ' << value(srcml_unit_get_hash(unit.get()));
            std::cout << ' ' << value(srcml_unit_get_filename(unit.get()));
            std::cout << '\n';

            // total LOC
            LOC += srcml_unit_get_loc(unit.get());
        }
        std::cout << "units: " << numUnits << '\n';
        std::cout << "LOC: " << LOC << '\n';
    }

    void srcml_display_info(srcml_archive* srcml_arch, bool long_info) {

        auto nsSize = srcml_archive_get_namespace_size(srcml_arch);
        bool isarchive = !srcml_archive_is_solitary_unit(srcml_arch);

        for (size_t i = 0; i < nsSize; ++i) {
            if (srcml_archive_get_namespace_uri(srcml_arch, i)) {
                auto prefix = srcml_archive_get_namespace_prefix(srcml_arch, i);
                auto uri = srcml_archive_get_namespace_uri(srcml_arch, i);
                std::cout << "xmlns";
                if (prefix[0] != 0) {
                    std::cout << ':' << prefix;
                }
                std::cout << "=\"" << uri << "\"\n";
            }
        }

        auto xml_encoding = srcml_archive_get_xml_encoding(srcml_arch);
        if (xml_encoding)
            std::cout << "encoding=" << "\"" << xml_encoding << "\"\n";

        std::unique_ptr<srcml_unit> unit(srcml_archive_read_unit(srcml_arch));
        int unit_count = 0;

        if (!isarchive && unit) {
            auto language = srcml_unit_get_language(unit.get());
            if (language)
                std::cout << "language=" << "\"" << language << "\"\n";
        }

        auto url = srcml_archive_get_url(srcml_arch);
        if (url) {
            std::cout << "url=" << "\"" << url << "\"\n";
        }

        if (!isarchive && unit) {
            auto filename = srcml_unit_get_filename(unit.get());
            if (filename)
                std::cout << "filename=" << "\"" << filename << "\"\n";
        }

        if (long_info) {

            ++unit_count;

            while (srcml_archive_skip_unit(srcml_arch)) {

                ++unit_count;
            }

            std::cout << "units=\"" << unit_count << "\"\n";
        }
    }

    int srcml_unit_count(srcml_archive* srcml_arch) {

        int numUnits = 0;
        while (true) {

            std::unique_ptr<srcml_unit> unit(srcml_archive_read_unit(srcml_arch));
            if (!unit)
                break;

            if (srcml_unit_get_language(unit.get()))
                ++numUnits;
        }

        return numUnits;
    }

}

void srcml_display_metadata(const srcml_request_t& srcml_request, const srcml_input_t& src_input, const srcml_output_dest&) {

    static const int display_commands =  SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
                                         SRCML_COMMAND_DISPLAY_SRCML_URL |
                                         SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
                                         SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
                                         SRCML_COMMAND_DISPLAY_SRCML_ENCODING |
                                         SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP |
                                         SRCML_COMMAND_DISPLAY_SRCML_HASH;

    for (const auto& input : src_input) {
        // create the output srcml archive
        OpenFileLimiter::open();
        std::unique_ptr<srcml_archive> srcml_arch(srcml_archive_create());

        int status = SRCML_STATUS_OK;
        if (contains<int>(input)) {
            status = srcml_archive_read_open_fd(srcml_arch.get(), input);
        }
        else if (contains<FILE*>(input)){
            status = srcml_archive_read_open_FILE(srcml_arch.get(), input);
        } else {
            status = srcml_archive_read_open_filename(srcml_arch.get(), (src_prefix_resource(input).c_str()));
        }
        if (status != SRCML_STATUS_OK) {
            SRCMLstatus(ERROR_MSG, "srcml input cannot not be opened.");
            return;
        }

        // Overrides all others Perform a pretty output
        if (srcml_request.pretty_format) {
            srcml_pretty(srcml_arch.get(), *srcml_request.pretty_format, srcml_request);
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
            const char* prefix = srcml_archive_get_prefix_from_uri(srcml_arch.get(), srcml_request.xmlns_prefix_query->c_str());
            if (prefix) {
                std::cout << prefix << '\n';
            }
        }

        if (!pretty_meta_header.empty() || !pretty_meta_body.empty())
            srcml_pretty(srcml_arch.get(), pretty_meta_header + " { " + pretty_meta_body + " } ", srcml_request);

        // units
        if (option(SRCML_COMMAND_UNITS))
            std::cout << srcml_unit_count(srcml_arch.get()) << "\n";

        // srcml info
        if (option(SRCML_COMMAND_INFO))
            srcml_display_info(srcml_arch.get(), false);

        // srcml long info
        if (option(SRCML_COMMAND_LONGINFO))
            srcml_display_info(srcml_arch.get(), true);

        if (option(SRCML_COMMAND_LIST))
            srcml_list(srcml_arch.get());
    }
}
