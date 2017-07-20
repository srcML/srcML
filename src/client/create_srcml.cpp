/**
 * @file create_srcml.hpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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

#include <functional>
#include <create_srcml.hpp>
#include <srcml.h>
#include <parse_queue.hpp>
#include <write_queue.hpp>
#include <srcml_consume.hpp>
#include <srcml_write.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_file.hpp>
#include <src_input_filesystem.hpp>
#include <src_input_filelist.hpp>
#include <src_input_stdin.hpp>
#include <src_input_text.hpp>
#include <src_prefix.hpp>
#include <srcml_input_srcml.hpp>
#include <trace_log.hpp>
#include <srcml_options.hpp>
#include <input_file.hpp>
#include <curl_input_file.hpp>
#include <input_curl.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <input_archive.hpp>

bool createdsrcml = false;

int srcml_handler_dispatch(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          const srcml_input_src& input,
                          const srcml_output_dest& destination) {

    // call appropriate handler
    if (input.state == SRCML) {

        // compressions require a per-input decompression stage
        srcml_input_src uninput = input;
        input_file(uninput);
        return srcml_input_srcml(queue, srcml_arch, uninput, srcml_request.revision);

    } else if (input.protocol == "text") {

        return src_input_text(queue, srcml_arch, srcml_request, input);

    } else if (input.protocol == "filelist") {

        // always create the archive
        int status = 0;
        if (contains<int>(destination)) {

            status = srcml_archive_write_open_fd(srcml_arch, *destination.fd);

        } else {

            status = srcml_archive_write_open_filename(srcml_arch, destination.c_str(), 0);
        }
        if (status != SRCML_STATUS_OK)
            return 0;
        createdsrcml = true;

        int num = src_input_filelist(queue, srcml_arch, srcml_request, input, destination);

        return num;

    } else if (input.protocol == "file" && input.isdirectory) {

        return src_input_filesystem(queue, srcml_arch, srcml_request, input);

    } else if (input.protocol == "file" && input.archives.empty() && input.compressions.empty()) {
       
        return src_input_file(queue, srcml_arch, srcml_request, input);

    } else if (input.protocol != "file" && curl_supported(input.protocol) && input.extension == ".xml") { 

        // input must go through libcurl pipe
        srcml_input_src uninput = input;
        if (!input_curl(uninput))
            return 0;

        // may have some compressions/archives
        uninput.fd = input_archive(uninput);

        return srcml_input_srcml(queue, srcml_arch, uninput, srcml_request.revision);

    } else if (input.protocol != "file" && curl_supported(input.protocol)) { 

        // input must go through libcurl pipe
        srcml_input_src uninput = input;
        if (!input_curl(uninput))
            return 0;

        return src_input_libarchive(queue, srcml_arch, srcml_request, uninput);

    } else {

        return src_input_libarchive(queue, srcml_arch, srcml_request, input);
    }
}

// create srcml from the current request
void create_srcml(const srcml_request_t& srcml_request,
                  const srcml_input_t& input_sources,
                  const srcml_output_dest& destination) {

    // create the output srcml archive
    srcml_archive* srcml_arch = srcml_archive_create();

    // set options for the output srcml archive
    if (srcml_request.att_xml_encoding)
        srcml_archive_set_xml_encoding(srcml_arch, srcml_request.att_xml_encoding->c_str());

    if (srcml_request.src_encoding)
        srcml_archive_set_src_encoding(srcml_arch, srcml_request.src_encoding->c_str());

    // for single input src archives (e.g., .tar), filename attribute is the source filename (if not already given)
    if (srcml_request.att_url) {
        srcml_archive_set_url(srcml_arch, src_prefix_resource(*srcml_request.att_url).c_str());
    } else if (input_sources.size() == 1 && input_sources[0].archives.size() > 0) {

        // Cleanup filename
        std::string url_name = src_prefix_resource(input_sources[0].filename);
        while (url_name.at(0) == '.' || url_name.at(0) == '/') {
            url_name.erase(0,1);
        }
        
        srcml_archive_set_url(srcml_arch, url_name.c_str());
    }

    if (srcml_request.att_version)
        srcml_archive_set_version(srcml_arch, srcml_request.att_version->c_str());

    if (srcml_request.markup_options)
        srcml_archive_set_options(srcml_arch, srcml_archive_get_options(srcml_arch) | *srcml_request.markup_options);

    if (*srcml_request.markup_options & SRCML_OPTION_XML_DECL)
        srcml_archive_disable_option(srcml_arch, SRCML_OPTION_XML_DECL);
    else
        srcml_archive_enable_option(srcml_arch, SRCML_OPTION_XML_DECL);

    if (srcml_request.att_language)
        srcml_archive_set_language(srcml_arch, srcml_request.att_language->c_str());
    else
        srcml_archive_set_language(srcml_arch, SRCML_LANGUAGE_NONE);

    srcml_archive_set_tabstop(srcml_arch, srcml_request.tabs);

    // non-archive when:
    //   only one input
    //   no cli request to make it an archive
    //   not a directory (if local file)
    // TODO: check if a plain file. Source archives, i.e., .tar.gz, always produce srcml archives
    if (input_sources.size() == 1 && input_sources[0].protocol != "filelist" &&
        !(srcml_request.markup_options && (*srcml_request.markup_options & SRCML_ARCHIVE)) &&
        !input_sources[0].isdirectory && input_sources[0].archives.size() < 1) {

        srcml_archive_disable_full_archive(srcml_arch);
        
        // If --hash is used, force hash for single input
        if (*srcml_request.markup_options & SRCML_HASH) {
            srcml_archive_enable_hash(srcml_arch);
        }
        else {
            srcml_archive_disable_hash(srcml_arch);
        }

    } else {

        // if this is an archive, then no filename attribute is allowed
        if (srcml_request.att_filename) {
            fprintf(stderr, "Attribute filename cannot be set for a srcML archive\n");
            exit(SRCML_STATUS_INVALID_ARGUMENT);
        }

        srcml_archive_enable_full_archive(srcml_arch);
        srcml_archive_enable_hash(srcml_arch);
    }

    // rns
    for (const auto& ext : srcml_request.language_ext) {
        size_t pos = ext.find('=');
        srcml_archive_register_file_extension(srcml_arch, ext.substr(0, pos).c_str(), ext.substr(pos+1).c_str());
    }

    // register xml namespaces
    std::map<std::string, std::string>::const_iterator itr;
    for (const auto& ns : srcml_request.xmlns_namespaces) {
        srcml_archive_register_namespace(srcml_arch, ns.first.c_str(), ns.second.c_str());
    }

    // setup the parsing queue
    TraceLog log(SRCMLOptions::get());
    log.header();
    WriteQueue write_queue(std::bind(srcml_write_request, std::placeholders::_1, std::ref(log), destination), srcml_request.command & SRCML_COMMAND_OUTPUT_ORDERED);
    ParseQueue parse_queue(srcml_request.max_threads, std::bind(srcml_consume, std::placeholders::_1, &write_queue), write_queue);

    // process input sources
    int status = 0;
    for (const auto& input : input_sources) {

        int numhandled = srcml_handler_dispatch(parse_queue, srcml_arch, srcml_request, input, destination);
        if (!numhandled)
            status = 1;
    }

    // wait for the parsing and writing queues to finish
    parse_queue.wait();
    write_queue.wait();

    // send an EOS (End Of Stream) write request
    ParseRequest* eos = new ParseRequest();
    eos->srcml_arch = srcml_arch;
    eos->status = createdsrcml ? 2000 : 1000;
    write_queue.eos(eos);

    log.report();

    if (status == 1)
        exit(1);
}
