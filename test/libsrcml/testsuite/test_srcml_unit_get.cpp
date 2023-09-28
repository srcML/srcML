// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_unit_get.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_unit_get_*()
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

        dassert(srcml_unit_get_revision(unit), std::string(SRCML_VERSION_STRING));

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
        srcml_unit_set_version(unit, "1.5");

        dassert(srcml_unit_get_version(unit), std::string("1.5"));

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

        dassert(srcml_archive_has_hash(archive), 1);
        dassert(srcml_unit_get_hash(unit), std::string("a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"));

        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_get_hash(0), 0);
    }

    /*
      srcml_get_namespace_size
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_namespace_size(unit), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_register_namespace(unit, "foo1", "bar1");
        srcml_unit_register_namespace(unit, "foo2", "bar2");
        dassert(srcml_unit_get_namespace_size(unit), 2);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_get_namespace_size(0), 0);
    }

    /*
      srcml_unit_get_namespace_prefix
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_namespace_prefix(unit, 0), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_namespace_prefix(unit, 2), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_get_namespace_prefix(0, 0), 0);
    }

    /*
      srcml_unit_get_prefix_from_uri
    */
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_prefix_from_uri(unit, "http://www.srcML.org/srcML/src"), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_prefix_from_uri(unit, "bar3"), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_prefix_from_uri(unit, 0), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_get_prefix_from_uri(0, "http://www.srcML.org/srcML/cpp"), 0);
    }

    /*
      srcml_unit_get_namespace_uri
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_namespace_uri(unit, 0), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_namespace_uri(unit, 2), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_get_namespace_uri(0, 0), 0);
    }

    /*
      srcml_unit_get_uri_from_prefix
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_uri_from_prefix(unit, ""), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_uri_from_prefix(unit, "foo3"), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_unit_get_uri_from_prefix(unit, 0), 0);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_get_uri_from_prefix(0, ""), 0);
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
        dassert(srcml_archive_has_hash(archive), 1);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;", 2);

        dassert(srcml_unit_get_srcml_outer(unit), std::string(R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>)"));

        srcml_unit_free(unit);
    }

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
    {
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_get_srcml(unit), 0);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;", 2);

        dassert(srcml_unit_get_srcml(unit), std::string(R"(<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>)"));

        srcml_unit_free(unit);
    }

    srcml_archive_free(archive);

    srcml_cleanup_globals();

    return 0;
}
