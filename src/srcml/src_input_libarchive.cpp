/**
 * @file src_input_libarchive.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  src_input_libarchive assigns local files, stdin, and archival input to the
  srcml parsing queue
*/

#include <src_input_libarchive.hpp>
#include <iostream>
#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>

void setup_libarchive(archive* arch) {
    // Configure libarchive supported file formats
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
}

// Convert input to a ParseRequest and assign request to the processing queue
void src_input_libarchive(ParseQueue& queue, srcml_archive* srcml_arch, const std::string& input_file, const boost::optional<std::string>& lang, const boost::optional<std::string>& option_filename, const boost::optional<std::string>& option_directory, boost::optional<FILE*> fstdin) {

    // libArchive Setup
    archive* arch = archive_read_new();
    archive_entry* arch_entry;

    setup_libarchive(arch);

    // open the archive
    int open_status;
    if (fstdin)
        open_status = archive_read_open_FILE(arch, *fstdin);
    else if (input_file == "-")
        open_status = archive_read_open_filename(arch, 0, 16384);
    else
        open_status = archive_read_open_filename(arch, input_file.c_str(), 16384);
    if (open_status != ARCHIVE_OK) {
        std::cerr << "Unable to open file\n";
        exit(1);
    }

    const std::string& main_filename = option_filename ? *option_filename : (input_file != "" ? input_file : "-");

    bool empty = true;
    while (archive_read_next_header(arch, &arch_entry) == ARCHIVE_OK) {
        empty = false;

        // default is filename from archive entry
        std::string filename = archive_entry_pathname(arch_entry);

        // archive entry filename for non-archive input is "data"
        if (filename == "data")
            filename = main_filename;

        // language may have been explicitly set
        std::string language;

        if (lang)
            language = *lang;

        // if not explicitly set, language comes from extension
        if (language == "")
            if (const char* l = srcml_archive_check_extension(srcml_arch, filename.c_str()))
                language = l;

        // at this point there are no other language options
        if (language == "") {
            if (filename == "-" || fstdin)
                std::cerr << "Using stdin requires a declared language\n";
            else
                std::cerr << "Extension not supported\n";
            continue;
        }

        // form the parsing request
        ParseRequest request;
        request.filename = main_filename;
        request.srcml_arch = srcml_arch;
        request.lang = language;

        // fill up the parse request buffer
        request.buffer.clear();
        const char* buffer;
        size_t size;
        int64_t offset;
        while (archive_read_data_block(arch, (const void**) &buffer, &size, &offset) == ARCHIVE_OK)
            request.buffer.insert(request.buffer.end(), buffer, buffer + size);

        // Hand request off to the processing queue
        queue.push(request);
    }

    // If the input is empty
    if (empty) {
        ParseRequest request;
        request.filename = main_filename;
        request.srcml_arch = srcml_arch;
        request.lang = srcml_archive_get_language(srcml_arch) ? lang->c_str() : srcml_archive_check_extension(srcml_arch, input_file.c_str());
        queue.push(request);
    }

    archive_read_finish(arch);
}
