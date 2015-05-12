/*
  test_srcml_apply_transforms.cpp

  Copyright (C) 2013-2014 SDML (www.srcML.org)

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  Test cases for srcml_apply_transforms
*/

#include <srcml_macros.hpp>

#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

int main() {

    const std::string srcml_a = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n";
    const std::string srcml_b = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" directory=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n";
    const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";
    const std::string srcml_a_after = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" filename=\"a.cpp\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";
    const std::string srcml_b_after = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" directory=\"test\" filename=\"project\" version=\"1\">\n\n<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" directory=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";
    const std::string srcml_full = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\">\n\n<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" directory=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";
    const std::string srcml_full_python = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\">\n\n<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"Python\" directory=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

    std::string copy;
    {
        std::ifstream in("copy.xsl");
        char c = 0;
        while(in.get(c)) {
            copy += c;
        }
    }

    std::string setlanguage;
    {
        std::ifstream in("setlanguage.xsl");
        char c = 0;
        while(in.get(c)) {
            setlanguage += c;
        }
    }

    std::string schema;
    {
        std::ifstream in("schema.rng");
        char c = 0;
        while(in.get(c)) {
            schema += c;
        }
    }

    /*
      srcml_apply_transforms
    */

    /*
      xpath
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);
        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_enable_option(oarchive, SRCML_OPTION_ARCHIVE);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a_after);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_enable_option(oarchive, SRCML_OPTION_ARCHIVE);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b_after);
        free(s);

    }

    /*
      xslt_filename
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_filename(iarchive, "setlanguage.xsl");
        srcml_append_transform_param(iarchive, "language", "\"Python\"");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);

    }

    /*
      xslt_memory
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_memory(iarchive, setlanguage.c_str(), setlanguage.size());
        srcml_append_transform_param(iarchive, "language", "\"Python\"");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_memory(iarchive, setlanguage.c_str(), setlanguage.size());
        srcml_append_transform_stringparam(iarchive, "language", "Python");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);

    }

    /*
      xslt_FILE
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        FILE * f = fopen("setlanguage.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_append_transform_param(iarchive, "language", "\"Python\"");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);

    }

    /*
      xslt_fd
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        int fd = OPEN("setlanguage.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);
        srcml_append_transform_param(iarchive, "language", "\"Python\"");

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);

    }

    /*
      relaxng_filename
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);

    }

    /*
      relaxng_memory
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);

    }

    /*
      relaxng_FILE
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        FILE * f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        FILE * f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        FILE * f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        FILE * f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);

    }

    /*
      relaxng_fd
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);

    }

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);

    }

    /*
      all
    */

    {

        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());

        srcml_append_transform_xpath(iarchive, "//src:unit");

        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);

    }

    {
        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());

        srcml_append_transform_xpath(iarchive, "//src:unit");

        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);

    }

    {
        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());

        srcml_append_transform_xpath(iarchive, "//src:unit");

        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_enable_option(oarchive, SRCML_OPTION_ARCHIVE);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a_after);
        free(s);

    }

    {
        char * s;
        size_t size;
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());

        srcml_append_transform_xpath(iarchive, "//src:unit");

        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive * oarchive = srcml_archive_clone(iarchive);
        srcml_archive_enable_option(oarchive, SRCML_OPTION_ARCHIVE);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_apply_transforms(iarchive, oarchive);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b_after);
        free(s);

    }

    srcml_cleanup_globals();

    return 0;

}
