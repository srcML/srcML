/**
 * @file test_srcml_append_transform.cpp
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

  Test cases for srcml_append_transform_*
*/

#include <srcml_macros.hpp>

#include <stdio.h>
#include <cstring>
#include <cassert>

#include <srcml.h>

#include <string>
#include <fstream>
#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <unit_tests.hpp>

int main(int, char* argv[]) {

    std::ifstream in("copy.xsl");
    std::string copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    std::ifstream inschema("schema.rng");
    std::string schema((std::istreambuf_iterator<char>(inschema)), std::istreambuf_iterator<char>());

    std::string s = "<unit/>";

    /*
      srcml_append_transform_xpath
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xpath(archive, "//src:unit"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath(archive, "//src:unit"), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

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
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xpath_attribute(archive, "//src:unit", "foo", "bar", "name", "value"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xpath_attribute(archive, "//src:unit", "foo", "bar", "name", "value"), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xpath_attribute(archive, "//src:unit", "foo", "bar", 0, "value"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xpath_attribute(archive, 0, "foo", "bar", "name", "value"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_xpath_attribute(0, "//src:unit", "foo", "bar", "name", "value"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xpath_element
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xpath_element(archive, "//src:unit", "foo", "bar", "element"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xpath_element(archive, "//src:unit", "foo", "bar", "element"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_xpath_attribute(archive, "//src:unit", "foobar", "foobar2", "name", "value"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
 //       dassert(srcml_append_transform_xpath_element(archive, "//src:unit", "foo", "bar", "element"), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_xpath_element(archive, "//src:unit", "foo", "bar", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_xpath_element(archive, 0, "foo", "bar", "element"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_xpath_element(0, "//src:unit", "foo", "bar", "element"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xslt_filename
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());
        
        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

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
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());
        
        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size()), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size()), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xslt_memory(archive, 0, copy.size()), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

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
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        FILE * f = fopen("copy.xsl", "r");
        dassert(srcml_append_transform_xslt_FILE(archive, f), SRCML_STATUS_OK);
        fclose(f);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        FILE * f = fopen("copy.xsl", "r");
        dassert(srcml_append_transform_xslt_FILE(archive, f), SRCML_STATUS_OK);
        fclose(f);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xslt_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        FILE * f = fopen("copy.xsl", "r");
        dassert(srcml_append_transform_xslt_FILE(0, f), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(f);
    }

    /*
      srcml_append_transform_xslt_fd
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(archive, fd);
        CLOSE(fd);

//        dassert(archive->transformations.back().type, SRCML_XSLT);
//        assert(archive->transformations.back().doc != 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
//        dassert(srcml_append_transform_xslt_fd(archive, fd), SRCML_STATUS_INVALID_IO_OPERATION);
        CLOSE(fd);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xslt_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        dassert(srcml_append_transform_xslt_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
        CLOSE(fd);
    }

    /*
      srcml_append_transform_relaxng_filename
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_relaxng_filename(archive, "schema.rng"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_filename(archive, "schema.rng"), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

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
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size()), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size()), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_relaxng_memory(archive, 0, schema.size()), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

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
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        FILE * f = fopen("schema.rng", "r");
        dassert(srcml_append_transform_relaxng_FILE(archive, f), SRCML_STATUS_OK);
        fclose(f);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        FILE * f = fopen("schema.rng", "r");
        dassert(srcml_append_transform_relaxng_FILE(archive, f), SRCML_STATUS_OK);
        fclose(f);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_relaxng_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        FILE * f = fopen("schema.rng", "r");
        dassert(srcml_append_transform_relaxng_FILE(0, f), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(f);
    }

    /*
      srcml_append_transform_relaxng_fd
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        int fd = OPEN("schema.rng", O_RDONLY, 0);
        dassert(srcml_append_transform_relaxng_fd(archive, fd), SRCML_STATUS_OK);
        CLOSE(fd);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        int fd = OPEN("schema.rng", O_RDONLY, 0);
        dassert(srcml_append_transform_relaxng_fd(archive, fd), SRCML_STATUS_OK);
        CLOSE(fd);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_relaxng_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {

        int fd = OPEN("schema.rng", O_RDONLY, 0);
        dassert(srcml_append_transform_relaxng_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
        CLOSE(fd);
    }

    /*
      srcml_append_transform_param
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_param(archive, "foo", "bar"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_param(archive, "foo", "bar"), SRCML_STATUS_NO_TRANSFORMATION);

        srcml_archive_free(archive);
    }

    {

        dassert(srcml_append_transform_param(0, "foo", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_param(archive, 0, "bar"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_param(archive, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      srcml_append_transform_stringparam
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_stringparam(archive, "foo", "bar"), SRCML_STATUS_OK);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_append_transform_stringparam(archive, "foo", "bar"), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_archive_free(archive);
    }

    {
        dassert(srcml_append_transform_stringparam(0, "foo", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_stringparam(archive, 0, "bar"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_stringparam(archive, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      srcml_append_transform_*
    */

    {

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, s.c_str(), s.size());

        dassert(srcml_append_transform_xpath(archive, "//src:unit"), SRCML_STATUS_OK);

        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size()), SRCML_STATUS_OK);
        
        FILE * f = fopen("copy.xsl", "r");
        dassert(srcml_append_transform_xslt_FILE(archive, f), SRCML_STATUS_OK);
        fclose(f);
        
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        dassert(srcml_append_transform_xslt_fd(archive, fd), SRCML_STATUS_OK);
        CLOSE(fd);

        dassert(srcml_append_transform_param(archive, "foo", "bar"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_stringparam(archive, "foo", "bar"), SRCML_STATUS_OK);

        dassert(srcml_append_transform_relaxng_filename(archive, "schema.rng"), SRCML_STATUS_OK);
        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size()), SRCML_STATUS_OK);
        f = fopen("schema.rng", "r");
        dassert(srcml_append_transform_relaxng_FILE(archive, f), SRCML_STATUS_OK);
        fclose(f);

        fd = OPEN("schema.rng", O_RDONLY, 0);
        dassert(srcml_append_transform_relaxng_fd(archive, fd), SRCML_STATUS_OK);
        CLOSE(fd);

        srcml_archive_free(archive);
    }

    srcml_cleanup_globals();

    return 0;
}
