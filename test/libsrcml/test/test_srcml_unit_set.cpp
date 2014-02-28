/**
 * @file test_srcml_unit_set.cpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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

  Test cases for srcml_unit_set_*.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include "dassert.hpp"

int main() {

    srcml_archive * archive = srcml_create_archive();

    /*
      srcml_unit_set_encoding
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_encoding(unit, 0);
        dassert(unit->encoding, 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_encoding(unit, "foo");
        dassert(*unit->encoding, "foo");
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_set_encoding(0, "foo"), SRCML_STATUS_ERROR);
    }

    /*
      srcml_unit_set_language
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_language(unit, 0);
        dassert(unit->language, 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_language(unit, "foo");
        dassert(*unit->language, "foo");
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_set_language(0, "foo"), SRCML_STATUS_ERROR);
    }

    /*
      srcml_unit_set_filename
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_filename(unit, 0);
        dassert(unit->filename, 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_filename(unit, "foo");
        dassert(*unit->filename, "foo");
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_set_filename(0, "foo"), SRCML_STATUS_ERROR);
    }

    /*
      srcml_unit_set_directory
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_directory(unit, 0);
        dassert(unit->directory, 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_directory(unit, "foo");
        dassert(*unit->directory, "foo");
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_set_directory(0, "foo"), SRCML_STATUS_ERROR);
    }

    /*
      srcml_unit_set_version
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_version(unit, 0);
        dassert(unit->version, 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_version(unit, "foo");
        dassert(*unit->version, "foo");
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_set_version(0, "foo"), SRCML_STATUS_ERROR);
    }

    srcml_free_archive(archive);

    return 0;
}
