/**
 * @file test_srcml_unit_get.cpp
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

  Test cases for srcml_unit_get_*.
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
      srcml_unit_get_encoding
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->encoding = boost::optional<std::string>();
        dassert(srcml_unit_get_encoding(unit), 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->encoding = "foo";
        dassert(srcml_unit_get_encoding(unit), std::string("foo"));
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_get_encoding(0), 0);
    }

    /*
      srcml_unit_get_language
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->language = boost::optional<std::string>();
        dassert(srcml_unit_get_language(unit), 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->language = "foo";
        dassert(srcml_unit_get_language(unit), std::string("foo"));
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_get_language(0), 0);
    }

    /*
      srcml_unit_get_filename
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->filename = boost::optional<std::string>();
        dassert(srcml_unit_get_filename(unit), 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->filename = "foo";
        dassert(srcml_unit_get_filename(unit), std::string("foo"));
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_get_filename(0), 0);
    }

    /*
      srcml_unit_get_directory
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->directory = boost::optional<std::string>();
        dassert(srcml_unit_get_directory(unit), 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->directory = "foo";
        dassert(srcml_unit_get_directory(unit), std::string("foo"));
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_get_directory(0), 0);
    }

    /*
      srcml_unit_get_version
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->version = boost::optional<std::string>();
        dassert(srcml_unit_get_version(unit), 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->version = "foo";
        dassert(srcml_unit_get_version(unit), std::string("foo"));
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_get_version(0), 0);
    }

    /*
      srcml_unit_get_xml
    */

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->unit = boost::optional<std::string>();
        dassert(srcml_unit_get_xml(unit), 0);
        srcml_free_unit(unit);
    }

    {

        srcml_unit * unit = srcml_create_unit(archive);
        unit->unit = "<unit/>";
        dassert(srcml_unit_get_xml(unit), std::string("<unit/>"));
        srcml_free_unit(unit);
    }

    {
        const char * s = "<unit/>";

        srcml_archive * iarchive = srcml_create_archive();
        srcml_read_open_memory(iarchive, s, strlen(s));
        srcml_unit * unit = srcml_read_unit_header(iarchive);
        dassert(srcml_unit_get_xml(unit), std::string("<unit/>"));
        srcml_free_unit(unit);
        srcml_close_archive(iarchive);
        srcml_free_archive(iarchive);
    }

    {
        const char * s = "<unit/>";

        srcml_archive * iarchive = srcml_create_archive();
        srcml_read_open_memory(iarchive, s, strlen(s));
        srcml_unit * unit = srcml_create_unit(iarchive);
        dassert(srcml_unit_get_xml(unit), 0);
        srcml_free_unit(unit);
        srcml_close_archive(iarchive);
        srcml_free_archive(iarchive);
    }

    {
        srcml_unit * unit = srcml_create_unit(archive);
        dassert(srcml_unit_get_xml(unit), 0);
        srcml_free_unit(unit);
    }

    {
        dassert(srcml_unit_get_xml(0), 0);
    }

    srcml_free_archive(archive);

    return 0;
}
