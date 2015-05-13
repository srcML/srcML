/**
 * @file test_srcml_unit_get.cpp
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

  Test cases for srcml_unit_get_*.
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
      srcml_unit_get_src_encoding
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->encoding = boost::optional<std::string>();
        dassert(srcml_unit_get_src_encoding(unit), 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->encoding = "foo";
        dassert(srcml_unit_get_src_encoding(unit), std::string("foo"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_src_encoding(0), 0);
    }

    /*
      srcml_unit_get_revision
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_revision(unit), std::string(SRCML_VERSION_STRING));
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->revision = boost::optional<std::string>();
        dassert(srcml_unit_get_revision(unit), 0);
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_revision(0), 0);
    }


    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->revision = "foo";
        dassert(srcml_unit_get_revision(unit), std::string("foo"));
        srcml_unit_free(unit);
    }

    /*
      srcml_unit_get_language
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->language = boost::optional<std::string>();
        dassert(srcml_unit_get_language(unit), 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->language = "foo";
        dassert(srcml_unit_get_language(unit), std::string("foo"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_language(0), 0);
    }

    /*
      srcml_unit_get_filename
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->filename = boost::optional<std::string>();
        dassert(srcml_unit_get_filename(unit), 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->filename = "foo";
        dassert(srcml_unit_get_filename(unit), std::string("foo"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_filename(0), 0);
    }

    /*
      srcml_unit_get_url
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->url = boost::optional<std::string>();
        dassert(srcml_unit_get_url(unit), 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->url = "foo";
        dassert(srcml_unit_get_url(unit), std::string("foo"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_url(0), 0);
    }

    /*
      srcml_unit_get_version
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->version = boost::optional<std::string>();
        dassert(srcml_unit_get_version(unit), 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->version = "foo";
        dassert(srcml_unit_get_version(unit), std::string("foo"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_version(0), 0);
    }

    /*
      srcml_unit_get_timestamp
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->timestamp = boost::optional<std::string>();
        dassert(srcml_unit_get_timestamp(unit), 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->timestamp = "foo";
        dassert(srcml_unit_get_timestamp(unit), std::string("foo"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_timestamp(0), 0);
    }

    /*
      srcml_unit_get_hash
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->hash = boost::optional<std::string>();
        dassert(srcml_unit_get_hash(unit), 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->hash = "foo";
        dassert(srcml_unit_get_hash(unit), std::string("foo"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_hash(0), 0);
    }

    /*
      srcml_unit_get_xml_fragment
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = boost::optional<std::string>();
        dassert(srcml_unit_get_xml_fragment(unit), 0);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        dassert(srcml_unit_get_xml_fragment(unit), std::string("<unit/>"));
        srcml_unit_free(unit);
    }

    {
        const char * s = "<unit/>";

        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, s, strlen(s));
        srcml_unit * unit = srcml_read_unit_header(iarchive);
        dassert(srcml_unit_get_xml_fragment(unit), std::string("<unit/>"));
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        const char * s = "<unit/>";

        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, s, strlen(s));
        srcml_unit * unit = srcml_unit_create(iarchive);
        dassert(srcml_unit_get_xml_fragment(unit), 0);
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        srcml_unit * unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_xml_fragment(unit), 0);
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_xml_fragment(0), 0);
    }

    /*
      srcml_unit_get_xml_standalone
    */

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = boost::optional<std::string>();
        char * xml = 0;
        size_t size = 0;
        dassert(srcml_unit_get_xml_standalone(unit, "UTF-8", &xml, &size), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        char * xml = 0;
        size_t size = 0;
        srcml_unit_get_xml_standalone(unit, 0, &xml, &size);
        dassert(xml, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\"/>"));
        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        char * xml = 0;
        size_t size = 0;
        srcml_unit_get_xml_standalone(unit, "UTF-8", &xml, &size);
        dassert(xml, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\"/>"));
        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit>\xc3\xbf<unit/>";
        char * xml = 0;
        size_t size = 0;
        srcml_unit_get_xml_standalone(unit, "ISO-8859-1", &xml, &size);
        dassert(xml, std::string("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\">\xff</unit>"));
        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    {
        const char * s = "<unit/>";

        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, s, strlen(s));
        srcml_unit * unit = srcml_read_unit_header(iarchive);
        char * xml = 0;
        size_t size = 0;
        srcml_unit_get_xml_standalone(unit, "UTF-8", &xml, &size);
        dassert(xml, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\"/>"));
        srcml_memory_free(xml);
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        const char * s = "<unit/>";

        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, s, strlen(s));
        srcml_unit * unit = srcml_unit_create(iarchive);
        char * xml = 0;
        size_t size = 0;
        dassert(srcml_unit_get_xml_standalone(unit, "UTF-8", &xml, &size), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_memory_free(xml);
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        srcml_unit * unit = srcml_unit_create(archive);
        char * xml = 0;
        size_t size = 0;       
        dassert(srcml_unit_get_xml_standalone(unit, "UTF-8", &xml, &size), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    {
        char * xml = 0;
        size_t size = 0; 
        dassert(srcml_unit_get_xml_standalone(0, 0, &xml, &size), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        char * xml = 0;
        size_t size = 0;
        dassert(srcml_unit_get_xml_standalone(unit, 0, 0, &size), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    {

        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        char * xml = 0;
        size_t size = 0;
        dassert(srcml_unit_get_xml_standalone(unit, 0, &xml, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    srcml_archive_free(archive);

    srcml_cleanup_globals();

    return 0;
}
