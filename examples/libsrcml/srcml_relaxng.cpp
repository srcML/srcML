// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_relaxng.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * RelaxNG usage.
*/

#include <srcml.h>

int main(int argc, char * argv[]) {

    // open the input archive
    srcml_archive* inputArchive = srcml_archive_create();
    srcml_archive_read_open_filename(inputArchive, "project.xml");

    // add the RelaxNG validation
    srcml_append_transform_relaxng_filename(inputArchive, "schema.rng");

    // open the output archive
    srcml_archive* outputArchive = srcml_archive_clone(inputArchive);
    srcml_archive_write_open_filename(outputArchive, "relaxng.xml");

    // apply the RelaxNG validation to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(inputArchive))) {

        // apply the transform
        // since the RelaxNG is just a validiation, convert the unit in-place
        srcml_unit_apply_transforms(inputArchive, unit, nullptr);

        // write the transformed unit
        srcml_archive_write_unit(outputArchive, unit);

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
