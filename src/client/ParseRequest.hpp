// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ParseRequest.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef PARSE_REQUEST_HPP
#define PARSE_REQUEST_HPP

#include <srcml.h>
#include <string>
#include <vector>
#include <srcml_utilities.hpp>
#include <memory>
#include <optional>

struct ParseRequest {

    // Fields required by thread to process a unit
    std::string language;
    std::optional<std::string> filename;
    std::optional<std::string> url;
    std::optional<std::string> version;
    std::vector<char> buffer;
    srcml_archive* srcml_arch = nullptr;
    std::unique_ptr<srcml_unit> unit;
    std::optional<std::string> disk_filename;
    std::optional<std::string> disk_dir;
    std::string parsertest_filename;
    int position = 0;
    int status = 0;
    double runtime = 0;
    std::optional<std::string> time_stamp;
    std::optional<std::string> errormsg;
    bool needsparsing = true;
    srcml_transform_result* results = nullptr;
    std::shared_ptr<srcml_archive> input_archive;
};

#endif
