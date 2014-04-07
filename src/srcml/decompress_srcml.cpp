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

    return;

    archive* ar = archive_read_new();

    // just a bunch of bytes
    archive_read_support_format_raw(ar);

    // setup decompressions
//    BOOST_FOREACH(const std::string& ext, input_sources[0].compressions)
//        archive_read_support_compression_by_extension(ar, ext.c_str());

    int status = ARCHIVE_OK;
    if (contains<int>(destination)) {
        status = archive_read_open_fd(ar, destination, 16384);
    } else {
        status = archive_read_open_filename(ar, destination.resource.c_str(), 16384);
    }
    if (status != ARCHIVE_OK) {
        std::cerr << status;
        exit(1);
    }
    
    archive_read_close(ar);
    archive_read_finish(ar);
}
