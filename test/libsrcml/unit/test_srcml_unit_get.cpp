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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*

  Test cases for srcml_unit_get_*.
*/

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    srcml_archive* archive = srcml_archive_create();
    srcml_archive_enable_hash(archive);

    /*
      srcml_unit_get_src_encoding
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_src_encoding(unit), 0);
        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, "foo");
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
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_revision(unit), std::string(SRCML_VERSION_STRING));
        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_revision(unit), std::string("1.0.0"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_revision(0), 0);
    }

   /*
      srcml_unit_get_language
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_language(unit), 0);
        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        dassert(srcml_unit_get_language(unit), std::string("C++"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_language(0), 0);
    }

    /*
      srcml_unit_get_filename
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_filename(unit), 0);
        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "main.cpp");
        dassert(srcml_unit_get_filename(unit), std::string("main.cpp"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_filename(0), 0);
    }

    /*
      srcml_unit_get_version
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_version(unit), 0);
        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, "foo");
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
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_timestamp(unit), 0);
        srcml_unit_free(unit);
    }

    {
        // @todo What is a valid timestamp?
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, "Fri Nov 30 14:15:27 EST 2018");
        dassert(srcml_unit_get_timestamp(unit), std::string("Fri Nov 30 14:15:27 EST 2018"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_timestamp(0), 0);
    }

    /*
      srcml_unit_get_hash
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_hash(unit), 0);
        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;", 2);
        assert(srcml_archive_has_hash(archive));
        dassert(srcml_unit_get_hash(unit), std::string("a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"));
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_hash(0), 0);
    }

    /*
      srcml_unit_get_srcml_outer
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_srcml_outer(unit), 0);
        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        assert(srcml_archive_has_hash(archive));
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;", 2);
        dassert(srcml_unit_get_srcml_outer(unit), std::string(R"(<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>)"));
        srcml_unit_free(unit);
    }
/*
    {
        const char * s = "<unit/>";

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, s, strlen(s));
        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        dassert(srcml_unit_get_srcml_outer(unit), std::string("<unit/>"));
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        const char * s = "<unit/>";

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, s, strlen(s));
        srcml_unit* unit = srcml_unit_create(iarchive);
        dassert(srcml_unit_get_srcml_outer(unit), 0);
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }
*/
    {
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_srcml_outer(unit), 0);
        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_srcml_outer(0), 0);
    }

    /*
      srcml_unit_get_srcml
    */
/*
    {
        srcml_unit* unit = srcml_unit_create(archive);
        char * xml = 0;
        size_t size = 0;
        dassert(srcml_unit_get_srcml(unit, "UTF-8", &xml, &size), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }
*/

    {
        srcml_unit* unit = srcml_unit_create(archive);
        assert(srcml_archive_has_hash(archive));
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;", 2);
        dassert(srcml_unit_get_srcml(unit), std::string(R"(<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>)"));
        srcml_unit_free(unit);
    }
/*
    {
        srcml_unit* unit = srcml_unit_create(archive);
//        unit->srcml = "<unit>\xc3\xbf<unit/>";
        char * xml = 0;
//        size_t size = 0;
//        srcml_unit_get_srcml(unit, "ISO-8859-1", &xml, &size);
        dassert(xml, std::string("<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="" SRCML_VERSION_STRING "">\xff</unit>"));
//        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }
*/
    /*
    {
        const char * s = "<unit/>";

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, s, strlen(s));
        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        char * xml = 0;
//        size_t size = 0;
//        srcml_unit_get_srcml(unit, "UTF-8", &xml, &size);
        dassert(xml, std::string("<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="" SRCML_VERSION_STRING ""/>"));
//        srcml_memory_free(xml);
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }
*/
    /*
    {
        const char * s = "<unit/>";

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, s, strlen(s));
        srcml_unit* unit = srcml_unit_create(iarchive);
//        char * xml = 0;
//        size_t size = 0;
//        dassert(srcml_unit_get_srcml(unit, "UTF-8", &xml, &size), SRCML_STATUS_INVALID_ARGUMENT);
//        srcml_memory_free(xml);
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
//        char * xml = 0;
//        size_t size = 0;       
//        dassert(srcml_unit_get_srcml(unit, "UTF-8", &xml, &size), SRCML_STATUS_INVALID_ARGUMENT);
//        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    {
//        char * xml = 0;
//        size_t size = 0; 
//        dassert(srcml_unit_get_srcml(0, 0, &xml, &size), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
//        unit->srcml = "<unit/>";
//        char * xml = 0;
//        size_t size = 0;
//        dassert(srcml_unit_get_srcml(unit, 0, 0, &size), SRCML_STATUS_INVALID_ARGUMENT);
//        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
//        unit->srcml = "<unit/>";
//        char * xml = 0;
//        size_t size = 0;
//        dassert(srcml_unit_get_srcml(unit, 0, &xml, 0), SRCML_STATUS_INVALID_ARGUMENT);
//        srcml_memory_free(xml);
        srcml_unit_free(unit);
    }
*/
    srcml_archive_free(archive);

    srcml_cleanup_globals();

    return 0;
}
