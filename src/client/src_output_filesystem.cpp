/**
 * @file src_output_filesystem.cpp
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

#include <src_output_filesystem.hpp>
#include <srcml.h>
#include <iostream>
#include <archive.h>
#include <archive_entry.h>

void src_output_filesystem(srcml_archive* srcml_arch, const std::string& output_dir, TraceLog& log) {

    // construct the relative directory
    std::string prefix;
    if (output_dir != "." && output_dir != "./")
        prefix = output_dir;

    auto a = archive_write_disk_new();
    auto entry = archive_entry_new();
    archive_entry_set_filetype(entry, AE_IFDIR); 
    archive_entry_set_perm(entry, 0744);
    int count = 0;
    std::string last;
    while (srcml_unit* unit = srcml_archive_read_unit(srcml_arch)) {

        const char* cfilename = srcml_unit_get_filename(unit);
        // @todo What do we do with no filename?
        if (!cfilename)
            continue;

        std::string filename = cfilename;

        // separate pathname from filename
        std::string path = prefix;
        path += '/';
        auto pos = filename.rfind('/');
        path += pos != std::string::npos ? filename.substr(0, pos) : "";

        std::string fullfilename = prefix;
        fullfilename += "/";
        fullfilename += filename;

        // use libarchive to create the file path
        // @todo Can we get away with no permission on final file?
        if (last != path) {
            archive_entry_set_pathname(entry, path.c_str());
            archive_write_header(a, entry);
            archive_write_finish_entry(a);
        }
        last = path;

        // unparse directory to filename
        log << ++count << fullfilename;

        srcml_unit_unparse_filename(unit, fullfilename.c_str());

        srcml_unit_free(unit);
    }

    archive_entry_free(entry);
    archive_write_close(a);
    archive_write_free(a);
}
