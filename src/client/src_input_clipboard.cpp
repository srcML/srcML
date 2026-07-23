// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_clipboard.cpp
 *
 * @copyright Copyright (C) 2026; srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_input_clipboard.hpp>
#include <src_input_libarchive.hpp>
#include <SRCMLStatus.hpp>
#include <clip.h>
#include <memory>
#include <string>

int src_input_clipboard(ParseQueue& queue, srcml_archive* srcml_arch, const srcml_request_t& srcml_request, const srcml_input_src& input) {

    // modify passed input
    srcml_input_src clipboard_input = input;

    // no backing filename; use "-" so libarchive input emits no filename attribute
    clipboard_input.resource = "-";

    // read the clipboard
    clipboard_input.memory = std::make_shared<std::string>();
    if (!clip::get_text(*clipboard_input.memory)) {
        SRCMLstatus(ERROR_MSG, "srcml: unable to read text from the clipboard");
        return -1;
    }

    return src_input_libarchive(queue, srcml_arch, srcml_request, clipboard_input);
}
