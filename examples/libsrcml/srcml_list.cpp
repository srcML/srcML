// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_list.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Gather info from an archive from a given unit.
 */

#include <srcml.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    srcml_archive* archive = srcml_archive_create();
    srcml_archive_read_open_filename(archive, "project.xml");

    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(archive))) {
        const char* filename = srcml_unit_get_filename(unit);
        if (filename)
            std::cout << filename << '\n';

        srcml_unit_free(unit);
    }

    srcml_archive_close(archive);
    srcml_archive_free(archive);

    return 0;
}
