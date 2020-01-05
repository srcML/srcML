/**
 * @file src_output_libarchive.cpp
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

#include <src_output_libarchive.hpp>
#include <srcml.h>
#include <archive.h>
#include <archive_entry.h>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include <SRCMLStatus.hpp>
#include <memory>
#include <libarchive_utilities.hpp>
#include <srcml_utilities.hpp>

void src_output_libarchive(srcml_archive* srcml_arch, archive* src_archive) {

    ssize_t arch_status = ARCHIVE_OK;
    int unitcounter = 0;
    while (std::unique_ptr<srcml_unit> unit{srcml_archive_read_unit(srcml_arch)}) {

        ++unitcounter;

        // have to make sure we have a valid filename
        std::string newfilename = srcml_unit_get_filename(unit.get()) ? srcml_unit_get_filename(unit.get()) : "";
        if (newfilename.empty()) {
            newfilename = "srcml_unit_";
            newfilename += std::to_string(unitcounter);
            if (language_to_std_extension(srcml_unit_get_language(unit.get())) != "")
                newfilename += language_to_std_extension(srcml_unit_get_language(unit.get()));
            SRCMLstatus(WARNING_MSG, "A srcML unit without a filename saved as " + newfilename);
        }

        // setup the entry header
        std::unique_ptr<archive_entry> entry(archive_entry_new());
        if (!entry)
            break;

        // Convert from srcML back to source in a buffer
        char* buffer;
        size_t buffer_size;
        srcml_unit_unparse_memory(unit.get(), &buffer, &buffer_size);
        std::unique_ptr<char> pbuffer(buffer);

        // setup the entry
        archive_entry_set_pathname(entry.get(), newfilename.c_str());
        archive_entry_set_size(entry.get(), buffer_size);
        archive_entry_set_filetype(entry.get(), AE_IFREG);
        archive_entry_set_perm(entry.get(), 0644);

        time_t now = time(nullptr);
        archive_entry_set_atime(entry.get(), now, 0);
        archive_entry_set_ctime(entry.get(), now, 0);
        archive_entry_set_mtime(entry.get(), now, 0);

        if ((arch_status = archive_write_header(src_archive, entry.get())) != ARCHIVE_OK)
            break;

        // write the data into the archive
        arch_status = archive_write_data(src_archive, pbuffer.get(), (size_t) buffer_size);
        if (arch_status == -1) {
            SRCMLstatus(WARNING_MSG, "Unable to save " + newfilename + " to source archive");
            break;
        }
    }
}
