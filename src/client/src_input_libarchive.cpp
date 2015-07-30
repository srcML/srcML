/**
 * @file src_input_libarchive.cpp
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

#ifdef _MSC_BUILD 
#define ssize_t __int64
#endif

#include <src_input_libarchive.hpp>
#include <srcml_options.hpp>
#include <curl/curl.h>
#include <archive.h>
#include <archive_entry.h>
#include <algorithm>
#include <timer.hpp>

#include <curl_input_file.hpp>

archive* libarchive_input_file(const srcml_input_src& input_file) {

    archive* arch = archive_read_new();

    archive_read_support_format_ar(arch);
    archive_read_support_format_cpio(arch);
    archive_read_support_format_gnutar(arch);
    archive_read_support_format_iso9660(arch);
    archive_read_support_format_mtree(arch);
    archive_read_support_format_tar(arch);
    archive_read_support_format_xar(arch);
    archive_read_support_format_zip(arch);
    archive_read_support_format_raw(arch);
    archive_read_support_format_empty(arch);

    /* Check libarchive version enable version specific features/syntax */
#if ARCHIVE_VERSION_NUMBER < 3000000
    // V2 Only Settings
    // Compressions
    archive_read_support_compression_all(arch);
#else
    // V3 Only Settings
    // File Formats
    archive_read_support_format_7zip(arch);
    archive_read_support_format_cab(arch);
    archive_read_support_format_lha(arch);
    archive_read_support_format_rar(arch);

    // Compressions
    archive_read_support_filter_all(arch);
#endif

    int status;
    curl curling;
    if (contains<int>(input_file)) {

        status = archive_read_open_fd(arch, input_file, 16384);

    } else if (contains<FILE*>(input_file)) {

        status = archive_read_open_FILE(arch, input_file);

    } else if (input_file.protocol != "file" && curl_supported(input_file.protocol)) {

        curling.source = input_file.filename;
        status = archive_read_open(arch, &curling, archive_curl_open, archive_curl_read, archive_curl_close);

    } else {

        status = archive_read_open_filename(arch, input_file.c_str(), 16384);
    }

    if (status != ARCHIVE_OK) {
        std::cerr << "Unable to open file " << input_file.filename << '\n';
        exit(1);
    }

    return arch;
}

// Convert input to a ParseRequest and assign request to the processing queue
void src_input_libarchive(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          const srcml_input_src& input_file) {

    // don't process if non-archive, non-compressed, and we don't handle the extension
    // this is to prevent trying to open, with srcml_archive_open_filename(), a non-srcml file,
    // which then hangs
    // Note: may need to fix in libsrcml
    if (!contains<int>(input_file) && !contains<FILE*>(input_file) && input_file.compressions.empty() && input_file.archives.empty() && !srcml_check_extension(input_file.plainfile.c_str())) {

        // if we are not verbose, then just end this attemp
        if (!(SRCML_COMMAND_VERBOSE & SRCMLOptions::get())) {
            return;
        }

        // form the parsing request
        ParseRequest* prequest = new ParseRequest;
        prequest->filename = input_file.resource;
        prequest->url = srcml_request.att_url;
        prequest->version = srcml_request.att_version;
        prequest->srcml_arch = srcml_arch;
        prequest->language = "";
        prequest->status = SRCML_STATUS_UNSET_LANGUAGE;
        prequest->total_num_inputs = srcml_request.input_sources.size();

        // schedule for parsing
        queue.schedule(prequest);

        return;
    }

    archive* arch = libarchive_input_file(input_file);

    /* In general, go through this once for each time the header can be read
       Exception: if empty, go through the loop exactly once */
    int count = 0;
    archive_entry *entry;
    int status = ARCHIVE_OK;
    while (status == ARCHIVE_OK &&
           (((status = archive_read_next_header(arch, &entry)) == ARCHIVE_OK) ||
            (status == ARCHIVE_EOF && !count))) {

        // skip any directories
        if (status == ARCHIVE_OK && archive_entry_filetype(entry) == AE_IFDIR)
            continue;

        // default is filename from archive entry (if not empty)
        std::string filename = status == ARCHIVE_OK ? archive_entry_pathname(entry) : "";

        // stdin, single files require a explicit filename
        if (filename == "data" && !srcml_request.att_language && input_file.filename == "stdin://-") {
            std::cerr << "Language required for stdin single files" << '\n';
            exit(1);
        }

        if (count == 0 && filename != "data" && status != ARCHIVE_EOF) {
            srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
            srcml_archive_enable_option(srcml_arch, SRCML_OPTION_HASH);
        }

        // archive entry filename for non-archive input is "data"
        if (filename.empty() || filename == "data")
            filename = input_file.resource;

        if (srcml_request.att_filename && !(srcml_archive_get_options(srcml_arch) & SRCML_OPTION_ARCHIVE))
            filename = *srcml_request.att_filename;

        // language may have been explicitly set
        std::string language;

        if (srcml_request.att_language)
            language = *srcml_request.att_language;

        // if not explicitly set, language comes from extension
        // we have to do this ourselves, since libsrcml can't for memory
        if (language.empty())
            if (const char* l = srcml_archive_check_extension(srcml_arch, filename.c_str()))
                language = l;

        // if we don't have a language, and are not verbose, then just end this attemp
        if (language.empty() && !(SRCML_COMMAND_VERBOSE & SRCMLOptions::get())) {
            ++count;
            continue;
        }

        // form the parsing request
        ParseRequest* prequest = new ParseRequest;

        if (srcml_request.command & SRCML_COMMAND_NOARCHIVE)
            prequest->disk_dir = srcml_request.output_filename;

        if (srcml_request.att_filename || (filename != "-"))
            prequest->filename = filename;


        prequest->url = srcml_request.att_url;
        prequest->version = srcml_request.att_version;
        prequest->srcml_arch = srcml_arch;
        prequest->language = language;
        prequest->status = !language.empty() ? 0 : SRCML_STATUS_UNSET_LANGUAGE;
        prequest->total_num_inputs = srcml_request.input_sources.size();

        if (SRCML_COMMAND_TIMESTAMP & SRCMLOptions::get()) {

            //Long time provided by libarchive needs to be time_t
            time_t mod_time(archive_entry_mtime(entry));

            //Standard ctime output and prune '/n' from string
            char* c_time = ctime(&mod_time);
            c_time[strlen(c_time) - 1] = 0;
            
            prequest->time_stamp = c_time;
        }

        // fill up the parse request buffer
        if (!status && !prequest->status) {
            // if we know the size, create the right sized data_buffer
            if (archive_entry_size_is_set(entry))
                prequest->buffer.reserve(archive_entry_size(entry));

            const char* buffer;
            size_t size;
#if ARCHIVE_VERSION_NUMBER < 3000000
            off_t offset;
#else
            int64_t offset;
#endif
            while (status == ARCHIVE_OK && archive_read_data_block(arch, (const void**) &buffer, &size, &offset) == ARCHIVE_OK)
                prequest->buffer.insert(prequest->buffer.end(), buffer, buffer + size);

            // LOC count
            prequest->loc = std::count(prequest->buffer.begin(), prequest->buffer.end(), '\n');
            if (!prequest->buffer.empty() && prequest->buffer.back() != '\n')
                ++prequest->loc;
        }
        
        // schedule for parsing
        queue.schedule(prequest);

        ++count;
    }
    archive_read_finish(arch);
}
