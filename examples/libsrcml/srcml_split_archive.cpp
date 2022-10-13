/**
 * @file srcml_split_archive.cpp
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

  Split an archive into two archives, one for include files and one for non-include files.
*/

#include <srcml.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    // open an existing archive in the file "project.xml"
    srcml_archive* inputArchive = srcml_archive_create();
    srcml_archive_read_open_filename(inputArchive, "project.xml");

    // create two new output srcML archives where the options and attributes
    // start the same as the original archive
    srcml_archive* includeArchive = srcml_archive_clone(inputArchive);
    srcml_archive* nonIncludeArchive = srcml_archive_clone(inputArchive);
    srcml_archive_write_open_filename(includeArchive, "projectInclude.xml");
    srcml_archive_write_open_filename(nonIncludeArchive, "projectNonInclude.xml");

    // copy the files from the input archive to the output archives
    // where the include files are written to the includeArchive
    // and the rest are written to the nonIncludeArchive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(inputArchive))) {

        // split the input archive into an include archive and a non-include archive
        const char* rawFilename = srcml_unit_get_filename(unit);
        if (!rawFilename) {
            std::cerr << "Unit missing filename\n";
            srcml_unit_free(unit);
            continue;
        }
        std::string filename = rawFilename;

        std::string extension;
        const auto extensionPosition = filename.rfind('.');
        if (extensionPosition != std::string::npos) {
            extension = filename.substr(extensionPosition);
        }
        if (extension == ".h" || extension == ".hpp" || extension == ".hxx" ||
            extension == ".H" || extension == ".h++" || extension == ".hh")
            srcml_archive_write_unit(includeArchive, unit);
        else
            srcml_archive_write_unit(nonIncludeArchive, unit);

        // free the input unit
        srcml_unit_free(unit);
    }

    // close the archives
    srcml_archive_close(includeArchive);
    srcml_archive_close(nonIncludeArchive);
    srcml_archive_close(inputArchive);

    // free the archives
    srcml_archive_free(includeArchive);
    srcml_archive_free(nonIncludeArchive);
    srcml_archive_free(inputArchive);

    return 0;
}
