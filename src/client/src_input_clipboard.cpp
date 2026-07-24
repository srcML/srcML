// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_clipboard.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_input_clipboard.hpp>
#include <issrcml.hpp>
#include <SRCMLStatus.hpp>
#include <clip.h>
#include <memory>
#include <string>

void open_clipboard(srcml_input_src& input_source) {

    // no backing filename; use "-" so the input emits no filename attribute
    input_source.resource = "-";

    // read the clipboard
    input_source.memory = std::make_shared<std::string>();
    if (!clip::get_text(*input_source.memory)) {
        SRCMLstatus(ERROR_MSG, "srcml: unable to read text from the clipboard");
        exit(1);
    }

    // determine source or srcML based on the content
    input_source.issrcML = issrcML(*input_source.memory);
}
