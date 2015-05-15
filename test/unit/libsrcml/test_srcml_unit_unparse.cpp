/**
 * @file test_srcml_unit_unparse.cpp
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

  Test cases for srcml_unit_unparse
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

int write_callback(void * context, const char * buffer, size_t len) {

    return (int)fwrite(buffer, 1, len, (FILE *)context);

}

int close_callback(void * context UNUSED) {

    return 0;

}

int main() {

    const std::string src = "a;\n";
    const std::string src_macro = "MACRO1;\nMACRO2;\n";
    const std::string utf8_src = "/* \u2713 */\n";
    const std::string latin_src = "/* \xfe\xff */\n";

    const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

    const std::string utf8_srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \u2713 */</comment>\n</unit>\n";

    const std::string latin_srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \u00fe\u00ff */</comment>\n</unit>\n";

    const std::string latin_from_utf8_srcml = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* &#10003; */</comment>\n</unit>\n";

    const std::string latin_from_latin_srcml = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \xfe\xff */</comment>\n</unit>\n";

    const std::string srcml_macro = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" dir=\"test\" filename=\"project\" version=\"1\"><macro-list token=\"MACRO1\" type=\"src:macro\"/><macro-list token=\"MACRO2\" type=\"src:macro\"/>\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><macro><name>MACRO1</name></macro><empty_stmt>;</empty_stmt>\n<macro><name>MACRO2</name></macro><empty_stmt>;</empty_stmt>\n</unit>\n\n</unit>\n";

    const std::string srcml_macro_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><macro-list token=\"MACRO1\" type=\"src:macro\"/><macro-list token=\"MACRO2\" type=\"src:macro\"/><macro><name>MACRO1</name></macro><empty_stmt>;</empty_stmt>\n<macro><name>MACRO2</name></macro><empty_stmt>;</empty_stmt>\n</unit>\n";

    std::ofstream srcml_file("project.xml");
    srcml_file << srcml;
    srcml_file.close();

    std::ofstream srcml_utf8_file("project_utf8.xml");
    srcml_utf8_file << utf8_srcml;
    srcml_utf8_file.close();

    std::ofstream srcml_latin_file("project_latin.xml");
    srcml_latin_file << latin_srcml;
    srcml_latin_file.close();

    std::ofstream srcml_latin_from_utf8_file("project_latin_from_utf8.xml");
    srcml_latin_from_utf8_file << latin_from_utf8_srcml;
    srcml_latin_from_utf8_file.close();

    std::ofstream srcml_latin_from_latin_file("project_latin_from_latin.xml");
    srcml_latin_from_latin_file << latin_from_latin_srcml;
    srcml_latin_from_latin_file.close();

    std::ofstream srcml_macro_file("project_macro.xml");
    srcml_macro_file << srcml_macro;
    srcml_macro_file.close();

    std::ofstream srcml_macro_single_file("project_macro_single.xml");
    srcml_macro_single_file << srcml_macro_single;
    srcml_macro_single_file.close();

    /*
      srcml_unit_unparse_filename
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_unparse_filename(unit, "project.c", 0);
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_filename(unit, "project_utf8.cpp", 0);
        std::ifstream src_file("project_utf8.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_filename(unit, "project_latin_from_utf8.cpp", 0);
        std::ifstream src_file("project_latin_from_utf8.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_filename(unit, "project_latin.cpp", 0);
        std::ifstream src_file("project_latin.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_filename(unit, "project_latin_from_latin.cpp", 0);
        std::ifstream src_file("project_latin_from_latin.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_unparse_filename(unit, "project_macro.cpp", 0);
        std::ifstream src_file("project_macro.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_unparse_filename(unit, "project_macro_single.cpp", 0);
        std::ifstream src_file("project_macro_single.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        srcml_unit_unparse_filename(unit, "project.c", 0);
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_unit_create(archive);
        dassert(srcml_unit_unparse_filename(unit, "project.c", 0), SRCML_STATUS_UNINITIALIZED_UNIT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        dassert(srcml_unit_unparse_filename(unit, "project.c", 0), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unit_unparse_filename(unit, 0, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        dassert(srcml_unit_unparse_filename(0, "project.c", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    /*
      srcml_unit_unparse_memory
    */

    {

        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_unparse_memory(unit, &s, &size);
        dassert(s, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {

        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_memory(unit, &s, &size);
        dassert(s, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {

        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_memory(unit, &s, &size);
        dassert(s, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {

        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_memory(unit, &s, &size);
        dassert(s, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {

        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_memory(unit, &s, &size);
        dassert(s, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {

        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_unparse_memory(unit, &s, &size);
        dassert(s, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {

        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unit_unparse_memory(unit, &s, &size);
        dassert(s, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        srcml_unit_unparse_memory(unit, &s, &size);
        dassert(s, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_unit_create(archive);
        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_UNINITIALIZED_UNIT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {
        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unit_unparse_memory(unit, 0, &size), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {
        char *s;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unit_unparse_memory(unit, &s, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {
        char * s;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unit_unparse_memory(0, &s, &size), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    /*
      srcml_unit_unparse_FILE
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unit_unparse_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_utf8.cpp", "w");
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project_utf8.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_latin_from_utf8.cpp", "w");
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project_latin_from_utf8.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_latin.cpp", "w");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project_latin.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_latin_from_latin.cpp", "w");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project_latin_from_latin.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_macro.cpp", "w");
        srcml_unit_unparse_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project_macro.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_macro_single.cpp", "w");
        srcml_unit_unparse_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project_macro_single.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unit_unparse_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_unit_create(archive);
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_UNINITIALIZED_UNIT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_INVALID_IO_OPERATION);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unit_unparse_FILE(unit, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unit_unparse_FILE(0, file), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    /*
      srcml_unit_unparse_fd
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = OPEN("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_unparse_fd(unit, fd);
        CLOSE(fd);
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        UNLINK("project.c");

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = OPEN("project_utf8.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_fd(unit, fd);
        CLOSE(fd);
        std::ifstream src_file("project_utf8.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        UNLINK("project_utf8.cpp");

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = OPEN("project_latin_from_utf8.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_fd(unit, fd);
        CLOSE(fd);
        std::ifstream src_file("project_latin_from_utf8.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        UNLINK("project_latin_from_utf8.cpp");

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = OPEN("project_latin.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_fd(unit, fd);
        CLOSE(fd);
        std::ifstream src_file("project_latin.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        UNLINK("project_latin.cpp");

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = OPEN("project_latin_from_latin.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_fd(unit, fd);
        CLOSE(fd);
        std::ifstream src_file("project_latin_from_latin.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        UNLINK("project_latin_from_latin.cpp");

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = OPEN("project_macro.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_unparse_fd(unit, fd);
        CLOSE(fd);
        std::ifstream src_file("project_macro.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        UNLINK("project_macro.cpp");

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = OPEN("project_macro_single.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_unparse_fd(unit, fd);
        CLOSE(fd);
        std::ifstream src_file("project_macro_single.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        UNLINK("project_macro_single.cpp");

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        int fd = OPEN("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_unparse_fd(unit, fd);
        CLOSE(fd);
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        UNLINK("project.c");

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_unit_create(archive);
        int fd = OPEN("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_UNINITIALIZED_UNIT);
        CLOSE(fd);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        int fd = OPEN("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_INVALID_IO_OPERATION);
        CLOSE(fd);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unit_unparse_fd(unit, -1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = OPEN("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        dassert(srcml_unit_unparse_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
        CLOSE(fd);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    /*
      srcml_unit_unparse_io
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback);
        fclose(file);
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_utf8.cpp", "w");
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback);
        fclose(file);
        std::ifstream src_file("project_utf8.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_latin_from_utf8.cpp", "w");
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback);
        fclose(file);
        std::ifstream src_file("project_latin_from_utf8.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_latin.cpp", "w");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback);
        fclose(file);
        std::ifstream src_file("project_latin.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_latin_from_latin.cpp", "w");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback);
        fclose(file);
        std::ifstream src_file("project_latin_from_latin.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_macro.cpp", "w");
        srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback);
        fclose(file);
        std::ifstream src_file("project_macro.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project_macro_single.cpp", "w");
        srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback);
        fclose(file);
        std::ifstream src_file("project_macro_single.cpp");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback);
        fclose(file);
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_unit_create(archive);
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_UNINITIALIZED_UNIT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_INVALID_IO_OPERATION);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unit_unparse_io(unit, 0, write_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unit_unparse_io(unit, (void *)file, 0, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unit_unparse_io(0, (void *)file, write_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    UNLINK("project.c");
    UNLINK("project.xml");
    UNLINK("project_utf8.cpp");
    UNLINK("project_utf8.xml");
    UNLINK("project_latin.cpp");
    UNLINK("project_latin.xml");
    UNLINK("project_latin_from_utf8.cpp");
    UNLINK("project_latin_from_utf8.xml");
    UNLINK("project_latin_from_latin.cpp");
    UNLINK("project_latin_from_latin.xml");
    UNLINK("project_macro.cpp");
    UNLINK("project_macro.xml");
    UNLINK("project_macro_single.cpp");
    UNLINK("project_macro_single.xml");

    srcml_cleanup_globals();

    return 0;
}
