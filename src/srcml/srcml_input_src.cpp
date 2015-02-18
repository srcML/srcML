/**
 * @file srcml_input_src.cpp
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

#include <srcml_input_src.hpp>

int srcml_read_callback(void* context, char * buffer, size_t len) {
    archive* libarchive_srcml = (archive*) context;

    ssize_t status = archive_read_data(libarchive_srcml, (void*) buffer, len);

    return (int) status;
}

int srcml_close_callback(void* context) {
    archive* libarchive_srcml = (archive*) context;

    archive_read_close(libarchive_srcml);
    archive_read_finish(libarchive_srcml);

    return 0;
}

int srcml_archive_read_open(srcml_archive* arch, const srcml_input_src& input_source) {

    int status;
    if (input_source.arch)
        status = srcml_archive_read_open_io(arch, input_source.arch, srcml_read_callback, srcml_close_callback);
    else if (contains<int>(input_source))
        status = srcml_archive_read_open_fd(arch, input_source);
    else if (contains<FILE*>(input_source))
        status = srcml_archive_read_open_FILE(arch, input_source);
    else
        status = srcml_archive_read_open_filename(arch, input_source.c_str());

    return status;
}
