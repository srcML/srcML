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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef INCLUDED_PARSERTEST_HPP
#define INCLUDED_PARSERTEST_HPP

#include <ParseRequest.hpp>
#include <srcml.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

class ParserTest {
public:
    static const int FIELD_WIDTH_LANGUAGE = 12;
    static const int FIELD_WIDTH_URL = 30;

    ParserTest() { line_count = 0; count = 0; total = 0; failed = 0; }

    static void entry(const ParseRequest* request, srcml_archive* archive, srcml_unit* unit);

    static void report(srcml_archive* archive);

private:
    static std::string previous_filename;
    static int count;
    static int total;
    static int line_count;
    static std::string url;
    static std::map<std::string, int> ltotal;
    static std::map<std::string, int> misses;
    static int failed;
    static std::string unit_language;
    static std::vector<std::string> errors;
    static std::vector<std::string> summary;
};

#endif
