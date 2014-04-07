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
#include <boost/foreach.hpp>

 void decompress_srcml(const srcml_request_t& srcml_request,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    archive* ar = archive_read_new();

    // just a bunch of bytes
    archive_read_support_format_raw(ar);

    /* Check libarchive version enable version specific features/syntax */
#if ARCHIVE_VERSION_NUMBER < 3000000
    // V2 Only Settings
    // Compressions
    archive_read_support_compression_all(ar);

#else
    // V3 Only Settings
    // File Formats
    archive_read_support_format_7zip(ar);
    archive_read_support_format_cab(ar);
    archive_read_support_format_lha(ar);
    archive_read_support_format_rar(ar);

    // Compressions
    archive_read_support_filter_all(ar);
#endif    // setup decompressions

//    BOOST_FOREACH(const std::string& ext, input_sources[0].compressions)
//        archive_read_support_compression_by_extension(ar, ext.c_str());

    int status = ARCHIVE_OK;
    if (contains<int>(input_sources[0])) {
        status = archive_read_open_fd(ar, input_sources[0], 16384);
    } else {
        status = archive_read_open_filename(ar, input_sources[0].resource.c_str(), 16384);
    }
    if (status != ARCHIVE_OK) {
        std::cerr << status;
        exit(1);
    }
    archive_entry *entry;
    status = archive_read_next_header(ar, &entry);

    const char* buffer;
    size_t size;
#if ARCHIVE_VERSION_NUMBER < 3000000
    off_t offset;
#else
    int64_t offset;
#endif
    while (archive_read_data_block(ar, (const void**) &buffer, &size, &offset) == ARCHIVE_OK)
        write(*destination.fd, buffer, size);

    close(*destination.fd);

    archive_read_close(ar);
    archive_read_finish(ar);
}
