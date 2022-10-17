// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ParserTest.hpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
