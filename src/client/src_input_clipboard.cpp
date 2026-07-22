// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_clipboard.cpp
 *
 * @copyright Copyright (C) 2026; srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_input_clipboard.hpp>
#include <srcml_options.hpp>
#include <SRCMLStatus.hpp>
#include <clip.h>
#include <memory>
#include <string>

// Convert input to a ParseRequest and assign request to the processing queue
int src_input_clipboard(ParseQueue& queue, srcml_archive* srcml_arch, const srcml_request_t& srcml_request, const srcml_input_src& input) {

    // form the parsing request
    std::shared_ptr<ParseRequest> prequest(new ParseRequest);

    if (option(SRCML_COMMAND_NOARCHIVE))
        prequest->disk_dir = srcml_request.output_filename.resource;

    prequest->filename = srcml_request.att_filename;
    prequest->url = srcml_request.att_url;
    prequest->version = srcml_request.att_version;
    prequest->srcml_arch = srcml_arch;
    prequest->language = srcml_request.att_language ? *srcml_request.att_language : "";

    // if there is no language specified, then try to use the filename extension
    if (prequest->language.empty() && prequest->filename)
        if (const char* l = srcml_archive_check_extension(srcml_arch, prequest->filename->data()))
            prequest->language = l;

    prequest->status = 0;

    // fill the buffer from the clipboard
    std::string text;
    if (!clip::get_text(text)) {
        SRCMLstatus(ERROR_MSG, "srcml: unable to read text from the clipboard");
        return -1;
    }
    prequest->buffer.assign(text.begin(), text.end());

    // schedule for parsing
    queue.schedule(prequest);

    return 1;
}
