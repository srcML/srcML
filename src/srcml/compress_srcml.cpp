/**
 * @file compress_srcml.cpp
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

#include <compress_srcml.hpp>
#include <archive.h>
#include <boost/foreach.hpp>

void compress_srcml(const srcml_request_t& srcml_request,
                    const srcml_input_t& input_sources,
                    const srcml_output_dest& destination) {

    return;

    archive* ar = archive_write_new();

    // setup format
    BOOST_FOREACH(const std::string& ext, destination.archives)
        archive_write_set_format_by_extension(ar, ext.c_str());

    // setup compressions
    BOOST_FOREACH(const std::string& ext, destination.compressions)
        archive_write_set_compression_by_extension(ar, ext.c_str());

    int status = ARCHIVE_OK;
    if (contains<int>(destination)) {
        status = archive_write_open_fd(ar, destination);
    } else {
        status = archive_write_open_filename(ar, destination.resource.c_str());
    }
    if (status != ARCHIVE_OK) {
        std::cerr << status;
        exit(1);
    }
    
    archive_write_close(ar);
    archive_write_finish(ar);
}
