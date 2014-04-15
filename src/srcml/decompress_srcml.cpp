/**
 * @file decompress_srcml.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#include <decompress_srcml.hpp>
#include <archive.h>

void decompress_srcml(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    archive* libarchive_srcml = archive_read_new();

    // just a bunch of bytes
    archive_read_support_format_raw(libarchive_srcml);

    /* Check libarchive version enable version specific features/syntax */
#if ARCHIVE_VERSION_NUMBER < 3000000
    // V2 Only Settings
    // Compressions
    archive_read_support_compression_all(libarchive_srcml);

#else
    // V3 Only Settings
    // File Formats
    archive_read_support_format_7zip(libarchive_srcml);
    archive_read_support_format_cab(libarchive_srcml);
    archive_read_support_format_lha(libarchive_srcml);
    archive_read_support_format_rar(libarchive_srcml);

    // Compressions
    archive_read_support_filter_all(libarchive_srcml);
#endif    // setup decompressions

//    BOOST_FOREACH(const std::string& ext, input_sources[0].compressions)
//        archive_read_support_compression_by_extension(ar, ext.c_str());

    int status = ARCHIVE_OK;
    if (contains<int>(input_sources[0])) {
        status = archive_read_open_fd(libarchive_srcml, input_sources[0], 16384);
    } else {
        status = archive_read_open_filename(libarchive_srcml, input_sources[0].resource.c_str(), 16384);
    }
    if (status != ARCHIVE_OK) {
        std::cerr << status;
        exit(1);
    }
    archive_entry *entry;
    status = archive_read_next_header(libarchive_srcml, &entry);

    // copy from the libarchive decompressed data into the destination file descriptor
    // for the next stage in the pipeline
    const char* buffer;
    size_t size;
#if ARCHIVE_VERSION_NUMBER < 3000000
    off_t offset;
#else
    int64_t offset;
#endif
    while (archive_read_data_block(libarchive_srcml, (const void**) &buffer, &size, &offset) == ARCHIVE_OK)
        write(*destination.fd, buffer, size);

    // important to close, since this is how the file descriptor reader get an EOF
    close(*destination.fd);

    archive_read_close(libarchive_srcml);
    archive_read_finish(libarchive_srcml);
}
