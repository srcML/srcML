// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_append_transform.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_append_transform_*()
*/

#include <srcml.h>

#include <string>
#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    std::ifstream in("copy.xsl");
    std::string copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    std::ifstream inschema("schema.rng");
    std::string schema((std::istreambuf_iterator<char>(inschema)), std::istreambuf_iterator<char>());

    /*
      srcml_append_transform_xpath
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath(archive, "//src:unit"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath(archive, "//src:unit"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_xpath(0, "//src:unit"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xpath_attribute
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_attribute(archive, "//src:unit", "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_attribute(archive, "//src:unit", "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_attribute(archive, "//src:unit", "sup", "http://srcML.org/Supplement", 0, "supplement"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_attribute(archive, 0, "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_xpath_attribute(0, "//src:unit", "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xpath_element
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_element(archive, "//src:unit", "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_element(archive, "//src:unit", "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_xpath_attribute(archive, "//src:unit", "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_element(archive, "//src:unit", "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_element(archive, "//src:unit", "sup", "http://srcML.org/Supplement", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_element(archive, 0, "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_xpath_element(0, "//src:unit", "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_srcql
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql(archive, "$T $V;"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql(archive, "$T $V;"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_srcql(0, "$T $V;"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_srcql_attribute
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_attribute(archive, "$T $V;", "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_attribute(archive, "$T $V;", "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_attribute(archive, "$T $V;", "sup", "http://srcML.org/Supplement", 0, "supplement"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_attribute(archive, 0, "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_srcql_attribute(0, "$T $V;", "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_srcql_element
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_element(archive, "$T $V;", "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_element(archive, "$T $V;", "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_srcql_attribute(archive, "$T $V;", "sup", "http://srcML.org/Supplement", "type", "supplement"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_element(archive, "$T $V;", "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_element(archive, "$T $V;", "sup", "http://srcML.org/Supplement", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_srcql_element(archive, 0, "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_srcql_element(0, "$T $V;", "sup", "http://srcML.org/Supplement", "contain"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xslt_filename
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_filename(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_xslt_filename(0, "copy.xsl"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xslt_memory
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size()), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size()), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_memory(archive, 0, copy.size()), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_xslt_memory(0, copy.c_str(), copy.size()), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xslt_FILE
    */

    {
        srcml_archive* archive = srcml_archive_create();
        FILE* f = fopen("copy.xsl", "r");

        dassert(srcml_append_transform_xslt_FILE(archive, f), SRCML_STATUS_OK);

        fclose(f);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        FILE* f = fopen("copy.xsl", "r");

        dassert(srcml_append_transform_xslt_FILE(archive, f), SRCML_STATUS_OK);

        fclose(f);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
    }

    {
        FILE* f = fopen("copy.xsl", "r");

        dassert(srcml_append_transform_xslt_FILE(0, f), SRCML_STATUS_INVALID_ARGUMENT);

        fclose(f);
    }

    /*
      srcml_append_transform_xslt_fd
    */

    {
        srcml_archive* archive = srcml_archive_create();
        int fd = open("copy.xsl", O_RDONLY, 0);

        dassert(srcml_append_transform_xslt_fd(archive, fd), SRCML_STATUS_OK);

        close(fd);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        int fd = open("copy.xsl", O_RDONLY, 0);

        dassert(srcml_append_transform_xslt_fd(archive, fd), SRCML_STATUS_OK);

        close(fd);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        int fd = open("copy.xsl", O_RDONLY, 0);

        dassert(srcml_append_transform_xslt_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);

        close(fd);
    }

    /*
      srcml_append_transform_relaxng_filename
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_filename(archive, "schema.rng"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_filename(archive, "schema.rng"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_filename(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_relaxng_filename(0, "schema.rng"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_relaxng_memory
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size()), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size()), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_memory(archive, 0, schema.size()), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_relaxng_memory(0, schema.c_str(), schema.size()), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_relaxng_FILE
    */

    {
        srcml_archive* archive = srcml_archive_create();
        FILE* f = fopen("schema.rng", "r");

        dassert(srcml_append_transform_relaxng_FILE(archive, f), SRCML_STATUS_OK);

        fclose(f);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        FILE* f = fopen("schema.rng", "r");

        dassert(srcml_append_transform_relaxng_FILE(archive, f), SRCML_STATUS_OK);

        fclose(f);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        FILE* f = fopen("schema.rng", "r");

        dassert(srcml_append_transform_relaxng_FILE(0, f), SRCML_STATUS_INVALID_ARGUMENT);

        fclose(f);
    }

    /*
      srcml_append_transform_relaxng_fd
    */

    {
        srcml_archive* archive = srcml_archive_create();
        int fd = open("schema.rng", O_RDONLY, 0);

        dassert(srcml_append_transform_relaxng_fd(archive, fd), SRCML_STATUS_OK);

        close(fd);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        int fd = open("schema.rng", O_RDONLY, 0);

        dassert(srcml_append_transform_relaxng_fd(archive, fd), SRCML_STATUS_OK);

        close(fd);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        int fd = open("schema.rng", O_RDONLY, 0);

        dassert(srcml_append_transform_relaxng_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);

        close(fd);
    }

    /*
      srcml_append_transform_param
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_param(archive, "sup", "http://srcML.org/Supplement"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_param(archive, "sup", "http://srcML.org/Supplement"), SRCML_STATUS_NO_TRANSFORMATION);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {

        dassert(srcml_append_transform_param(0, "sup", "http://srcML.org/Supplement"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_param(archive, 0, "http://srcML.org/Supplement"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_param(archive, "sup", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      srcml_append_transform_stringparam
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_stringparam(archive, "sup", "http://srcML.org/Supplement"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_stringparam(archive, "sup", "http://srcML.org/Supplement"), SRCML_STATUS_NO_TRANSFORMATION);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_stringparam(0, "sup", "http://srcML.org/Supplement"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_stringparam(archive, 0, "http://srcML.org/Supplement"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_stringparam(archive, "sup", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      srcml_append_transform_*
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath(archive, "//src:unit"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size()), SRCML_STATUS_OK);

        FILE* f = fopen("copy.xsl", "r");
        dassert(srcml_append_transform_xslt_FILE(archive, f), SRCML_STATUS_OK);
        fclose(f);

        int fd = open("copy.xsl", O_RDONLY, 0);
        dassert(srcml_append_transform_xslt_fd(archive, fd), SRCML_STATUS_OK);
        close(fd);

        dassert(srcml_append_transform_param(archive, "sup", "http://srcML.org/Supplement"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_stringparam(archive, "sup", "http://srcML.org/Supplement"), SRCML_STATUS_OK);

        dassert(srcml_append_transform_relaxng_filename(archive, "schema.rng"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size()), SRCML_STATUS_OK);
        f = fopen("schema.rng", "r");
        dassert(srcml_append_transform_relaxng_FILE(archive, f), SRCML_STATUS_OK);
        fclose(f);

        fd = open("schema.rng", O_RDONLY, 0);
        dassert(srcml_append_transform_relaxng_fd(archive, fd), SRCML_STATUS_OK);
        close(fd);

        srcml_archive_free(archive);
    }

    srcml_cleanup_globals();

    return 0;
}
