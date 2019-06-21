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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*

  Test cases for srcml_unit_set_*.
*/

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    srcml_archive* archive = srcml_archive_create();

    /*
      srcml_unit_set_src_encoding
    */

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, 0);
//        dassert(unit->encoding, boost::none);
        srcml_unit_free(unit);
    }

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, "foo");
//        dassert(*unit->encoding, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_src_encoding(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_language
    */

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, 0);
//        dassert(unit->language, boost::none);
        srcml_unit_free(unit);
    }

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "foo");
//        dassert(*unit->language, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_language(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_filename
    */

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, 0);
//        dassert(unit->filename, boost::none);
        srcml_unit_free(unit);
    }

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "foo");
//        dassert(*unit->filename, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_filename(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_version
    */

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, 0);
//        dassert(unit->version, boost::none);
        srcml_unit_free(unit);
    }

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, "foo");
//        dassert(*unit->version, "foo");
        srcml_unit_free(unit);
    }

    {
        const char * v1 = "version1";
        const char * v2 = "version2";

        srcml_unit* unit1 = srcml_unit_create(archive);
        int status = srcml_unit_set_version(unit1, v1);
        dassert(status, SRCML_STATUS_OK);
       // dassert(*unit1->version, v1);
        assert(strcmp(srcml_unit_get_version(unit1), v1) == 0);

        srcml_unit* unit2 = srcml_unit_create(archive);
        status = srcml_unit_set_version(unit2, v2);
        dassert(status, SRCML_STATUS_OK);
//        dassert(*unit2->version, v2);
        assert(strcmp(srcml_unit_get_version(unit2), v2) == 0);

        srcml_archive_set_version(archive, "archiveVersion");

//        dassert(*unit1->version, v1);
        assert(strcmp(srcml_unit_get_version(unit1), v1) == 0);
//        dassert(*unit2->version, v2);
        assert(strcmp(srcml_unit_get_version(unit2), v2) == 0);

        srcml_unit_free(unit1);
        srcml_unit_free(unit2);
    }

    {
        dassert(srcml_unit_set_version(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_timestamp
    */

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, 0);
//        dassert(unit->timestamp, boost::none);
        srcml_unit_free(unit);
    }

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, "foo");
//        dassert(*unit->timestamp, "foo");
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_timestamp(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_unparse_set_eol
    */

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_unparse_set_eol(unit, SOURCE_OUTPUT_EOL_AUTO);
//        dassert(unit->eol, SOURCE_OUTPUT_EOL_AUTO);
        srcml_unit_free(unit);
    }

    {

        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_unparse_set_eol(unit, SOURCE_OUTPUT_EOL_CRLF);
//        dassert(unit->eol, SOURCE_OUTPUT_EOL_CRLF);
        srcml_unit_free(unit);
    }

    {

        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_unparse_set_eol(unit, SOURCE_OUTPUT_EOL_CRLF + 1), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_unparse_set_eol(0, SOURCE_OUTPUT_EOL_AUTO), SRCML_STATUS_INVALID_ARGUMENT);
    }

    srcml_archive_free(archive);

    return 0;
}
