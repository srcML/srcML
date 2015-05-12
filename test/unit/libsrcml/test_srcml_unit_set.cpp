/**
 * @file test_srcml_unit_set.cpp
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

  Test cases for srcml_unit_set_*.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

int main() {

    srcml_archive * archive = srcml_archive_create();

    /*
      srcml_unit_set_src_encoding
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, 0);
        dassert(unit->encoding, 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, "foo");
        dassert(*unit->encoding, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_src_encoding(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_language
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, 0);
        dassert(unit->language, 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "foo");
        dassert(*unit->language, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_language(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_filename
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, 0);
        dassert(unit->filename, 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "foo");
        dassert(*unit->filename, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_filename(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_url
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_url(unit, 0);
        dassert(unit->url, 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_url(unit, "foo");
        dassert(*unit->url, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_url(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_version
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, 0);
        dassert(unit->version, 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, "foo");
        dassert(*unit->version, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_version(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_timestamp
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, 0);
        dassert(unit->timestamp, 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, "foo");
        dassert(*unit->timestamp, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_timestamp(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_hash
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_hash(unit, 0);
        dassert(unit->hash, 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_hash(unit, "foo");
        dassert(*unit->hash, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_hash(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_unparse_set_eol
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_unparse_set_eol(unit, SRCML_UNPARSE_OPTION_AUTO);
        dassert(unit->eol, SRCML_UNPARSE_OPTION_AUTO);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_unparse_set_eol(unit, SRCML_UNPARSE_OPTION_CRLF);
        dassert(unit->eol, SRCML_UNPARSE_OPTION_CRLF);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        dassert(srcml_unit_unparse_set_eol(unit, SRCML_UNPARSE_OPTION_CRLF + 1), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_unparse_set_eol(0, SRCML_UNPARSE_OPTION_AUTO), SRCML_STATUS_INVALID_ARGUMENT);
    }

    srcml_archive_free(archive);

    return 0;
}
