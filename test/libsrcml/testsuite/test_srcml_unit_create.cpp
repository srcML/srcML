/**
 * @file test_srcml_unit_create.cpp
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

  Test cases for srcml_unit_create.
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
