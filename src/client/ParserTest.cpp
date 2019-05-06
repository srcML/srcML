/**
 * @file ParserTest.hpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
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

#include <ParseRequest.hpp>
#include <srcml.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <ParserTest.hpp>

void ParserTest::entry(const ParseRequest* request, srcml_unit* unit) {

    if (request->url)
        url = *request->url;

    if (previous_filename.empty() || (request->filename && *request->filename != previous_filename)) {

        if (request->filename)
            previous_filename = *request->filename;
        count = 0;

        unit_language = srcml_unit_get_language(unit);

        std::cout << '\n' << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << unit_language;
        std::cout << std::setw(FIELD_WIDTH_URL) << std::left << url;
        line_count = 0;
    }

    ++count;

    ++total;

    ++ltotal[unit_language];

    // get the src
    char* buffer = 0;
    size_t size = 0;
    srcml_unit_set_src_encoding(unit, "UTF-8");
    srcml_unit_unparse_memory(unit, &buffer, &size);

    // get the srcml
    const char* xml = srcml_unit_get_srcml_raw(unit);

    srcml_unit* outunit = srcml_unit_clone(unit);
    srcml_unit_set_language(outunit, srcml_unit_get_language(unit));

    srcml_unit_parse_memory(outunit, buffer, size);

    const char* sout = srcml_unit_get_srcml_raw(outunit);

    if (line_count >= 75) {
        std::cout << '\n' << std::setw(FIELD_WIDTH_LANGUAGE) << " " << std::setw(FIELD_WIDTH_URL) << std::left << "...";
        line_count = 0;
    }

    line_count += std::to_string(count).size() + 1;

    if (strcmp(sout, xml) == 0) {
        std::cout << "\033[0;33m" << count << "\033[0m";
    } else {
        ++failed;
        ++misses[unit_language];

        std::string errreport = unit_language;
        errreport += '\t';
        errreport += url;
        errreport += '\t';
        errreport += std::to_string(count);
        errreport += '\n';
        errreport += "\033[0;30;1m";
        errreport += "test:";
        errreport += "\033[0m";
        errreport += xml;
        errreport += "\033[0;30;1m";
        errreport += "srcml:";
        errreport += "\033[0m";
        errreport += sout;
        errreport += '\n';
        errors.push_back(errreport);
        std::cout << "\033[0;31m" << count << "\033[0m";
    }
    std::cout << " ";

    free(buffer);
    srcml_unit_free(outunit);
 //   srcml_unit_free(unit);

}

void ParserTest::report() {
    if (!total)
        return;

    // error report
    std::cout << "Errors:\n";
    for (const auto& err : errors) {
        std::cout << err;
    }
    double percent = double(failed * 100) / total;
    std::cout << "\033[0;30;1m" << "\nCounts: " << "\033[0m" << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << "Total" << std::setw(6) << std::right << failed << std::setw(6) << std::right << total << '\t' << std::setprecision(2) << percent << "%" << '\n';

    for (auto& kv : ltotal) {
        double percent = double(misses[kv.first] * 100) / kv.second;
        std::cout << "        " << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << kv.first << std::setw(6) << std::right << misses[kv.first] << std::setw(6) << std::right << kv.second << '\t' << std::setprecision(2) << percent << "%" << '\n';
    }
}

std::string ParserTest::previous_filename;
int ParserTest::count = 0;
int ParserTest::total = 0;
int ParserTest::line_count = 0;
std::string ParserTest::url;
std::map<std::string, int> ParserTest::ltotal;
std::map<std::string, int> ParserTest::misses;
int ParserTest::failed = 0;
std::string ParserTest::unit_language;
std::vector<std::string> ParserTest::errors;
