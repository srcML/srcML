/**
 * @file test_srcml_unit_create.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

/*

  Test cases for srcml_unit_create.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

int main() {

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);

        dassert(unit->archive, archive);
        dassert(unit->language, 0);
        dassert(unit->filename, 0);
        dassert(unit->url, 0);
        dassert(unit->version, 0);
        dassert(unit->timestamp, 0);
        dassert(unit->hash, 0);
        dassert(unit->unit, 0);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {

        dassert(srcml_unit_create(0), 0);

    }

    return 0;
}
