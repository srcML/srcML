/**
 * @file compress_srcml.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <compress_srcml.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <SRCMLStatus.hpp>
#include <libarchive_utilities.hpp>
#include <memory>

#if ARCHIVE_VERSION_NUMBER >= 3002000
void compress_srcml(const srcml_request_t& /* srcml_request */,
                    const srcml_input_t& input_sources,
                    const srcml_output_dest& destination) {

    // create a new archive for output that will handle all
    // types, including source-code files
    std::unique_ptr<archive> ar(archive_write_new());
    if (!ar) {
        SRCMLstatus(ERROR_MSG, "Unable to create libarchive archive for compression");
        exit(1);
    }
    archive_write_set_format_raw(ar.get());

    // setup compressions
    for (const auto& ext : destination.compressions)
        archive_write_set_compression_by_extension(ar.get(), ext.c_str());

    // open the new archive based on input source
    int status = ARCHIVE_OK;
    if (contains<int>(destination)) {
        status = archive_write_open_fd(ar.get(), destination);
    } else {
        status = archive_write_open_filename(ar.get(), destination.resource.c_str());
    }
    if (status != ARCHIVE_OK) {
        SRCMLstatus(ERROR_MSG, std::to_string(status));
        exit(1);
    }

    // create a new entry. Note that the pathname doesn't matter
    std::unique_ptr<archive_entry> entry(archive_entry_new());
    if (!entry) {
        SRCMLstatus(ERROR_MSG, "Unable to create libarchive entry for compression");
        exit(1);
    }
    archive_entry_set_pathname(entry.get(), "test");
    archive_entry_set_filetype(entry.get(), AE_IFREG);

    // create the header for our single entry
    // since this is output, and there is only a single output, any error is fatal
    if ((status = archive_write_header(ar.get(), entry.get())) != ARCHIVE_OK) {
        SRCMLstatus(ERROR_MSG, std::to_string(status));
        exit(1);
    }

    // write the data into the archive
    std::vector<char> buffer(4092);
    while (true) {
        ssize_t s = read(*input_sources[0].fd, buffer.data(), (size_t) buffer.size());
        if (s <= 0)
            break;

        ssize_t status = archive_write_data(ar.get(), buffer.data(), s);
        if (status == 0)
            break;
    }
}
#endif
