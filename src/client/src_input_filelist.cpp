/**
 * @file src_input_filelist.cpp
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

#include <src_input_filelist.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_file.hpp>
#include <src_input_filesystem.hpp>
#include <boost/foreach.hpp>
#include <create_srcml.hpp>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <archive.h>
#include <archive_entry.h>

void src_input_filelist(ParseQueue& queue,
                        srcml_archive* srcml_arch,
                        const srcml_request_t& srcml_request,
                        const std::string& input_file) {

    archive* arch = libarchive_input_file(input_file);

    archive_entry *entry = 0;
    int status = archive_read_next_header(arch, &entry);
    if (status != ARCHIVE_OK) {
    	std::cerr << "ERROR\n";
    	return;
    }

    // skip any directories
    if (archive_entry_filetype(entry) == AE_IFDIR) {
    	std::cerr << "ERROR\n";
    	return;
    }

    if (strcmp(archive_entry_pathname(entry), "data") != 0) {
    	std::cerr << "ERROR\n";
    	return;
    }

    // if we know the size, create the right sized data_buffer
    std::vector<char> vbuffer;
    if (archive_entry_size_is_set(entry))
        vbuffer.reserve(archive_entry_size(entry));

    const char* buffer;
    size_t size;
#if ARCHIVE_VERSION_NUMBER < 3000000
    off_t offset;
#else
    int64_t offset;
#endif

    // read the file into a buffer
    while (status == ARCHIVE_OK && archive_read_data_block(arch, (const void**) &buffer, &size, &offset) == ARCHIVE_OK)
           vbuffer.insert(vbuffer.end(), buffer, buffer + size);

    char* line = &vbuffer[0];
    while (line < &vbuffer[vbuffer.size() - 1]) {

    	// find the line
    	char* startline = line;
    	while (*line != '\n' && line != &vbuffer[vbuffer.size() - 1])
    		++line;
    	++line;

    	std::string sline(startline, line - startline);

		// skip comment lines
		if (sline[0] == '#')
			continue;

		// trim from both ends
		boost::algorithm::trim(sline);

		// skip empty lines
        if (sline[0] == 0)
            continue;

        // process this file
        srcml_handler_dispatch(queue, srcml_arch, srcml_request, sline);
    }
}
