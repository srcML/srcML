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
#include <sstream>
#include <cstring>
#include <algorithm>

#define str2arg(s) s, strlen(s)

void ParserTest::entry(const ParseRequest* request, srcml_archive* archive, srcml_unit* unit) {

    if (request->url)
        url = *request->url;

    if (previous_filename.empty() || (request->filename && *request->filename != previous_filename)) {

        if (request->filename)
            previous_filename = *request->filename;
        count = 0;

        unit_language = srcml_unit_get_language(unit);

        std::ostringstream sout;
        sout << '\n' << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << unit_language;
        sout << std::setw(FIELD_WIDTH_URL) << std::left << url;

        srcml_archive_write_string(archive, sout.str().c_str(), (int) sout.str().size());

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
    const char* xml = srcml_unit_get_srcml_inner(unit);

    srcml_unit* outunit = srcml_unit_clone(unit);
    srcml_unit_set_language(outunit, srcml_unit_get_language(unit));

    srcml_unit_parse_memory(outunit, buffer, size);

    const char* sout = srcml_unit_get_srcml_inner(outunit);

    if (line_count >= 75) {
        std::ostringstream sout;
        sout << '\n' << std::setw(FIELD_WIDTH_LANGUAGE) << " " << std::setw(FIELD_WIDTH_URL) << std::left << "...";
        srcml_archive_write_string(archive, sout.str().c_str(), (int) sout.str().size());

        line_count = 0;
    }

    line_count += (int)std::to_string(count).size() + 1;

    if (strcmp(sout, xml) == 0) {
        std::ostringstream sout;
        sout << "\033[0;33m" << count << "\033[0m";
        srcml_archive_write_string(archive, sout.str().c_str(), (int) sout.str().size());
    } else {
        ++failed;
        ++misses[unit_language];

        std::string sxml = xml;
        std::string ssout = sout;

        const int CUTOFF = 5;
        if (std::count(sxml.begin(), sxml.end(), '\n') > CUTOFF ||
            std::count(ssout.begin(), ssout.end(), '\n') > CUTOFF) {

            // find where the strings are different
            auto off = std::mismatch(sxml.begin(), sxml.end(), ssout.begin());
            while (off.first != sxml.begin() && *off.first != '\n') {
                off.first = std::prev(off.first);
                off.second = std::prev(off.second);
            }
            if (*off.first == '\n') {
                off.first = std::next(off.first);
                off.second = std::next(off.second);
            }
            sxml.erase(sxml.begin(), off.first);
            ssout.erase(ssout.begin(), off.second);

            std::reverse(sxml.begin(), sxml.end());
            std::reverse(ssout.begin(), ssout.end());

            off = std::mismatch(sxml.begin(), sxml.end(), ssout.begin());
            while (off.first != sxml.begin() && *off.first != '\n') {
                off.first = std::prev(off.first);
                off.second = std::prev(off.second);
            }
            if (*off.first == '\n') {
                off.first = std::next(off.first);
                off.second = std::next(off.second);
            }
            sxml.erase(sxml.begin(), off.first);
            ssout.erase(ssout.begin(), off.second);

            std::reverse(sxml.begin(), sxml.end());
            std::reverse(ssout.begin(), ssout.end());
        }

        std::string errreport = "\033[0;30;1m";
        errreport += unit_language;
        errreport += '\t';
        errreport += url;
        errreport += '\t';
        errreport += previous_filename;
        errreport += '\t';
        errreport += std::to_string(count);
        errreport += "\033[0m";
        errreport += '\n';
        errreport += "\033[0;30;1m";
        errreport += "test:\n";
        errreport += "\033[0m";
        errreport += sxml;
        errreport += "\033[0;30;1m";
        if (xml[strlen(xml) - 1] != '\n')
            errreport += '\n';
        errreport += "srcml:\n";
        errreport += "\033[0m";
        errreport += ssout;
        errreport += '\n';
        errors.push_back(errreport);

        std::ostringstream ossumreport;
        ossumreport << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << unit_language << std::setw(FIELD_WIDTH_URL) << std::left << url << " " << std::setw(FIELD_WIDTH_URL) << previous_filename << '\t';
        if (!summary.empty() && ossumreport.str() == summary.back().substr(0, ossumreport.str().size())) {
            ossumreport.str() = summary.back().substr(0, summary.back().size() - 1);
            ossumreport << ',';
            summary.pop_back();
        }
        ossumreport << count << '\n';
        summary.push_back(ossumreport.str());

        srcml_archive_write_string(archive, str2arg("\033[0;31m"));
        std::string result = std::to_string(count);
        srcml_archive_write_string(archive, result.c_str(), (int) result.size());
        srcml_archive_write_string(archive, str2arg("\033[0m"));
    }
    srcml_archive_write_string(archive, " ", 1);

    free(buffer);
    srcml_unit_free(outunit);
 //   srcml_unit_free(unit);

}

void ParserTest::report(srcml_archive* archive) {
    if (!total)
        return;

    // error report
    srcml_archive_write_string(archive, str2arg("\n\nErrors:\n"));
    for (const auto& err : errors) {
        srcml_archive_write_string(archive, err.c_str(), (int) err.size());
    }
    // summary report
    srcml_archive_write_string(archive, str2arg("\n\nSummary:\n"));
    std::sort(summary.begin(), summary.end());
    for (const auto& err : summary) {
        srcml_archive_write_string(archive, err.c_str(), (int) err.size());
    }
    double percent = double(failed * 100) / total;
    std::ostringstream sout;
    sout << "\033[0;30;1m" << "\nCounts: " << "\033[0m" << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << "Total" << std::setw(6) << std::right << failed << std::setw(6) << std::right << total << '\t' << std::setprecision(2) << percent << "%" << '\n';
    srcml_archive_write_string(archive, sout.str().c_str(), (int) sout.str().size());

    for (auto& kv : ltotal) {
        double percent = double(misses[kv.first] * 100) / kv.second;
        std::ostringstream sout;
        sout << "        " << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << kv.first << std::setw(6) << std::right << misses[kv.first] << std::setw(6) << std::right << kv.second << '\t' << std::setprecision(2) << percent << "%" << '\n';

        srcml_archive_write_string(archive, sout.str().c_str(), (int) sout.str().size());
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
std::vector<std::string> ParserTest::summary;
