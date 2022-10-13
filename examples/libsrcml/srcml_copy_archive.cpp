/**
 * @file srcml_copy_archive.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
  Example program of the use of the libsrcml C API.

  Copy an archive.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // open up an existing archive
    srcml_archive* inputArchive = srcml_archive_create();
    srcml_archive_read_open_filename(inputArchive, "project.xml");

    // create a new srcml archive structure
    // options and attributes of cloned archive start the same
    // original archive
    srcml_archive* outputArchive = srcml_archive_clone(inputArchive);

    // open a srcML archive for output
    srcml_archive_write_open_filename(outputArchive, "projectCopy.xml");

    // copy the files from the input archive to the output archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(inputArchive))) {

        // append to the output archive
        srcml_archive_write_unit(outputArchive, unit);

        srcml_unit_free(unit);
    }

    // close the archives
    srcml_archive_close(outputArchive);
    srcml_archive_close(inputArchive);

    // free the archives
    srcml_archive_free(outputArchive);
    srcml_archive_free(inputArchive);

    return 0;
}
