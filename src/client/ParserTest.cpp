// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ParserTest.hpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
#include <algorithm>
#include <srcml_cli.hpp>
#include <srcml_options.hpp>
#include <srcml_utilities.hpp>

void ParserTest::entry(const ParseRequest* request, srcml_archive* archive, srcml_unit* unit) {

    bool color = !(SRCMLOptions::get() & SRCML_COMMAND_NO_COLOR);

    if (request->url)
        url = *request->url;

    if (previous_filename.empty() || (request->parsertest_filename != previous_filename)) {

        previous_filename = request->parsertest_filename;
        count = 0;

        if (!srcml_unit_get_language(unit))
            return;

        unit_language = srcml_unit_get_language(unit);

        std::ostringstream sout;
        sout << '\n' << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << unit_language;
        sout << std::setw(FIELD_WIDTH_URL) << std::left << url;

        srcml_archive_write_string(archive, sout.str().data(), (int) sout.str().size());

        line_count = 0;
    }

    ++count;

    ++total;

    ++ltotal[unit_language];

    // get the src
    char* buffer = nullptr;
    size_t size = 0;
    srcml_unit_set_src_encoding(unit, "UTF-8");
    srcml_unit_unparse_memory(unit, &buffer, &size);

    // get the srcml
    std::string sxml;
    if (srcml_unit_get_srcml_inner(unit))
        sxml = srcml_unit_get_srcml_inner(unit);

    std::unique_ptr<srcml_unit> outunit(srcml_unit_clone(unit));
    srcml_unit_set_language(outunit.get(), srcml_unit_get_language(unit));

    srcml_unit_parse_memory(outunit.get(), buffer, size);

    std::string ssout;
    if (srcml_unit_get_srcml_inner(outunit.get()))
        ssout = srcml_unit_get_srcml_inner(outunit.get());

    if (line_count >= 75) {
        std::ostringstream sout;
        sout << '\n' << std::setw(FIELD_WIDTH_LANGUAGE) << " " << std::setw(FIELD_WIDTH_URL) << std::left << "...";
        srcml_archive_write_string(archive, sout.str().data(), (int) sout.str().size());

        line_count = 0;
    }

    line_count += (int)std::to_string(count).size() + 1;

    if (ssout == sxml) {
        std::ostringstream output;
        output << (color ? "\033[0;33m" : "") << count << (color ? "\033[0m" : "");
        srcml_archive_write_string(archive, output.str().data(), (int) output.str().size());
    } else {
        std::ostringstream output;
        output << (color ? "\033[0;31m" : "-") << count << (color ? "\033[0m" : "");
        srcml_archive_write_string(archive, output.str().data(), (int) output.str().size());

        ++failed;
        ++misses[unit_language];

        const int CUTOFF = 5;
        if (std::count(sxml.begin(), sxml.end(), '\n') > CUTOFF ||
            std::count(ssout.begin(), ssout.end(), '\n') > CUTOFF) {

            // find where the strings are different
            auto off = std::mismatch(sxml.begin(), sxml.end(), ssout.begin());

            // backup to right after the previous newline
            while (off.first != sxml.begin() && *off.first != '\n') {
                off.first = std::prev(off.first);
                off.second = std::prev(off.second);
            }
            if (*off.first == '\n') {
                off.first = std::next(off.first);
                off.second = std::next(off.second);
            }

            // remove the common start of the strings
            sxml.erase(sxml.begin(), off.first);
            ssout.erase(ssout.begin(), off.second);

            // find where the strings are different at the end
            const auto endoff = std::mismatch(sxml.rbegin(), sxml.rend(), ssout.rbegin());

            // backup to right before the previous newline
            while (off.first != sxml.begin() && *off.first != '\n') {
                off.first = std::prev(off.first);
                off.second = std::prev(off.second);
            }
            if (*off.first == '\n') {
                off.first = std::next(off.first);
                off.second = std::next(off.second);
            }

            // remove the common end of the strings
            sxml.resize(sxml.size() - std::distance(sxml.rbegin(), endoff.first));
            ssout.resize(ssout.size() - std::distance(ssout.rbegin(), endoff.second));
        }

        // record for the error report
        std::ostringstream error_report;
        error_report << (color ? "\033[0;30;1m" : "") << unit_language << '\t' << url << '\t' << previous_filename << '\t' << std::to_string(count) << (color ? "\033[0m" : "") << '\n' << (color ? "\033[0;30;1m" : "") << " "
                     << "test:\n" << (color ? "\033[0m" : "") << sxml << (color ? "\033[0;30;1m" : "");
         if (sxml.back() != '\n')
            error_report << '\n';
        error_report << "srcml:\n" << (color ? "\033[0m" : "") << ssout << '\n';
        errors.push_back(error_report.str());

        // record for the summary report
        std::ostringstream summary_report;
        summary_report << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << unit_language << std::setw(FIELD_WIDTH_URL) << std::left << url << " " << std::setw(FIELD_WIDTH_URL) << previous_filename << ' ';
        if (!summary.empty() && summary_report.str() == summary.back().substr(0, summary_report.str().size())) {
            summary_report << summary.back().substr(summary_report.str().size());
            summary_report << ',';
            summary.pop_back();
        }
        summary_report << count;
        summary.push_back(summary_report.str());
    }
    srcml_archive_write_string(archive, " ", 1);

    free(buffer);
}

void ParserTest::report(srcml_archive* archive) {
    if (!total)
        return;

    bool color = !(SRCMLOptions::get() & SRCML_COMMAND_NO_COLOR);

    // error report
    std::string_view errorsHeader = "\n\nErrors:\n";
    srcml_archive_write_string(archive, errorsHeader.data(), (int) errorsHeader.size());
    for (const auto& err : errors) {
        srcml_archive_write_string(archive, err.data(), (int) err.size());
    }
    // summary report
    std::string_view summaryHeader = "\n\nSummary:\n";
    srcml_archive_write_string(archive, summaryHeader.data(), (int) summaryHeader.size());
    std::sort(summary.begin(), summary.end());
    for (const auto& err : summary) {
        srcml_archive_write_string(archive, err.data(), (int) err.size());
        srcml_archive_write_string(archive, "\n", 1);
    }
    double percent = double(failed * 100) / total;
    std::ostringstream sout;
    sout << (color ? "\033[0;30;1m" : "") << "\nCounts: " << (color ? "\033[0m" : "") << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << "Total" << std::setw(6) << std::right << failed << std::setw(6) << std::right << total << '\t';
    sout << std::setw(4) << std::right;
    if (percent == 0 || percent >= 1)
        sout << std::setprecision(2) << percent;
    else
        sout << "<1";
    sout << "%" << '\n';
    srcml_archive_write_string(archive, sout.str().data(), (int) sout.str().size());

    for (auto& kv : ltotal) {
        double missesPercent = double(misses[kv.first] * 100) / kv.second;
        std::ostringstream out;
        out << "        " << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << kv.first << std::setw(6) << std::right << misses[kv.first] << std::setw(6) << std::right << kv.second << '\t';
        out << std::setw(4) << std::right;
        if (missesPercent == 0 || missesPercent >= 1)
            out << std::setprecision(2) << missesPercent;
        else
            out << "<1";
        out << "%" << '\n';

        srcml_archive_write_string(archive, out.str().data(), static_cast<int>(out.str().size()));
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
