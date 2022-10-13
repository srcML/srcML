/**
 * @file srcml_xpath.cpp
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

  Applies an XPath query. The query is an identity transformation,
  the entire transformation change produces an output identical to the input.

  Equivalent to the srcml command line:

  srcml -j 1 --xpath="//src:unit" project.xml -o xpath.xml
*/

#include <srcml.h>

int main(int argc, char * argv[]) {

    // open the input archive
    srcml_archive* inputArchive = srcml_archive_create();
    srcml_archive_read_open_filename(inputArchive, "project.xml");

    // add the xpath transformation
    srcml_append_transform_xpath(inputArchive, "//src:unit");

    // open the output archive
    srcml_archive* outputArchive = srcml_archive_clone(inputArchive);
    srcml_archive_write_open_filename(outputArchive, "xpath.xml");

    // apply the transformation to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(inputArchive))) {

        // apply the transforms
        // since an XPath transformation can produce multiple units
        // from a single input unit, this requires using the srcml_transform_result
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(inputArchive, unit, &result);

        // write the transform result
        for (int i = 0; i < srcml_transform_get_unit_size(result); ++i) {
            srcml_archive_write_unit(outputArchive, srcml_transform_get_unit(result, i));
        }

        // free the transformation result
        srcml_transform_free(result);

        // free the input unit
        srcml_unit_free(unit);
    }

    // close the archives
    srcml_archive_close(inputArchive);
    srcml_archive_close(outputArchive);

    // free the archives
    srcml_archive_free(inputArchive);
    srcml_archive_free(outputArchive);

    return 0;
}
