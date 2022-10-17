// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_unit_create.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_unit_create()
 */

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_get_language(unit), 0);
        dassert(srcml_unit_get_filename(unit), 0);
        dassert(srcml_unit_get_version(unit), 0);
        dassert(srcml_unit_get_timestamp(unit), 0);
        dassert(srcml_unit_get_hash(unit), 0);
        dassert(srcml_unit_get_srcml(unit), 0);
        dassert(srcml_unit_get_srcml_outer(unit), 0);
        dassert(srcml_unit_get_srcml_inner(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_create(0), 0);
    }

    return 0;
}
