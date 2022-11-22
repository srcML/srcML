// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_file.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_input_file.hpp>
#include <srcml_options.hpp>
#include <src_input_libarchive.hpp>

// Convert input to a ParseRequest and assign request to the processing queue
int src_input_file(ParseQueue& queue,
                    srcml_archive* srcml_arch,
                    const srcml_request_t& srcml_request,
                    const srcml_input_src& input) {

    if (option(SRCML_COMMAND_VERBOSE)) {
        return src_input_libarchive(queue, srcml_arch, srcml_request, input);
    }

    // form the parsing request
    std::shared_ptr<ParseRequest> prequest(new ParseRequest);

    if (option(SRCML_COMMAND_NOARCHIVE)) {
        prequest->disk_dir = srcml_request.output_filename.resource;
    }

    if (srcml_request.att_filename)
        prequest->filename = *srcml_request.att_filename;
    else if (input.resource != "_")
        prequest->filename = input.resource;

    prequest->url = srcml_request.att_url;
    prequest->version = srcml_request.att_version;
    prequest->srcml_arch = srcml_arch;
    prequest->language = srcml_request.att_language ? *srcml_request.att_language : "";

    if (prequest->language.empty())
        if (const char* l = srcml_archive_check_extension(srcml_arch, prequest->filename->c_str()))
            prequest->language = l;

    prequest->disk_filename = input.resource;

    // Hand request off to the processing queue
    queue.schedule(prequest);

    return 1;
}
