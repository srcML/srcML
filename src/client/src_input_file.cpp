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
    ParseRequest request;

    if (option(SRCML_COMMAND_NOARCHIVE)) {
        request.disk_dir = srcml_request.output_filename.resource;
    }

    if (srcml_request.att_filename)
        request.filename = *srcml_request.att_filename;
    else if (input.resource != "_")
        request.filename = input.resource;

    request.url = srcml_request.att_url;
    request.version = srcml_request.att_version;
    request.srcml_arch = srcml_arch;
    request.language = srcml_request.att_language ? *srcml_request.att_language : "";

    if (request.language.empty())
        if (const char* l = srcml_archive_check_extension(srcml_arch, request.filename->data()))
            request.language = l;

    request.disk_filename = input.resource;

    // Hand request off to the processing queue
    queue.schedule(std::move(request));

    return 1;
}
