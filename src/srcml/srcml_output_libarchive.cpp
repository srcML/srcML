/**
 * @file srcml_output_libarchive.cpp
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

#include <srcml_output_libarchive.hpp>
#include <srcml.h>
#include <archive.h>
#include <archive_entry.h>
#include <stdlib.h>

void srcml_output_libarchive(srcml_archive* srcml_arch, archive* src_archive) {

    int arch_status = ARCHIVE_OK;
    while (srcml_unit* unit = srcml_read_unit(srcml_arch)) {

        // setup the entry header
        archive_entry* entry = archive_entry_new();
        if (!entry)
            break;

        char* buffer;
        int buffer_size;
        srcml_unparse_unit_memory(unit, &buffer, &buffer_size);

        archive_entry_set_pathname(entry, srcml_unit_get_filename(unit));
        archive_entry_set_size(entry, buffer_size);
        archive_entry_set_filetype(entry, AE_IFREG);
        archive_entry_set_perm(entry, 0644);

        time_t now = time(NULL);
        archive_entry_set_atime(entry, now, 0);
        archive_entry_set_ctime(entry, now, 0);
        archive_entry_set_mtime(entry, now, 0);

        if ((arch_status = archive_write_header(src_archive, entry)) != ARCHIVE_OK)
            break;

        // write the data into the archive
        arch_status = archive_write_data(src_archive, buffer, (ssize_t)buffer_size);
        if (arch_status != ARCHIVE_OK || arch_status != buffer_size)
            break;

        free(buffer);

        archive_entry_free(entry);

        srcml_free_unit(unit);
    }
}
