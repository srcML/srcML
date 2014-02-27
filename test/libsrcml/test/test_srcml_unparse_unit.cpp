/**
 * @file test_srcml_unparse_unit.cpp
 *
 * @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
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

  Test cases for srcml_unparse_unit
*/
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

#include "dassert.hpp"

int main() {

    const std::string src = "a;\n";
    const std::string src_macro = "MACRO1;\nMACRO2;\n";
    const std::string utf8_src = "/* \u2713 */\n";
    const std::string latin_src = "/* \xfe\xff */\n";

    const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

    const std::string utf8_srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \u2713 */</comment>\n</unit>\n";

    const std::string latin_srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \u00fe\u00ff */</comment>\n</unit>\n";

    const std::string srcml_macro = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" dir=\"test\" filename=\"project\" version=\"1\"><macro-list token=\"MACRO1\" type=\"src:macro\"/><macro-list token=\"MACRO2\" type=\"src:macro\"/>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><macro><name>MACRO1</name></macro><empty_stmt>;</empty_stmt>\n<macro><name>MACRO2</name></macro><empty_stmt>;</empty_stmt>\n</unit>\n\n</unit>\n";

    const std::string srcml_macro_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><macro-list token=\"MACRO1\" type=\"src:macro\"/><macro-list token=\"MACRO2\" type=\"src:macro\"/><macro><name>MACRO1</name></macro><empty_stmt>;</empty_stmt>\n<macro><name>MACRO2</name></macro><empty_stmt>;</empty_stmt>\n</unit>\n";

    std::ofstream srcml_file("project.xml");
    srcml_file << srcml;
    srcml_file.close();

    std::ofstream srcml_utf8_file("project_utf8.xml");
    srcml_utf8_file << utf8_srcml;
    srcml_utf8_file.close();

    std::ofstream srcml_latin_file("project_latin.xml");
    srcml_latin_file << latin_srcml;
    srcml_latin_file.close();

    std::ofstream srcml_macro_file("project_macro.xml");
    srcml_macro_file << srcml_macro;
    srcml_macro_file.close();

    std::ofstream srcml_macro_single_file("project_macro_single.xml");
    srcml_macro_single_file << srcml_macro_single;
    srcml_macro_single_file.close();

    /*
      srcml_unparse_unit_filename
    */

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unparse_unit_filename(unit, "project.c");
        std::ifstream src_file("project.c");
        std::string aunit;
        char c = 0;
	while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
	srcml_unit_set_encoding(unit, "UTF-8");
        srcml_unparse_unit_filename(unit, "project_utf8.cpp");
        std::ifstream src_file("project_utf8.cpp");
	std::string aunit;
        char c = 0;
	while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, utf8_src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unparse_unit_filename(unit, "project_latin.cpp");
        std::ifstream src_file("project_latin.cpp");
	std::string aunit;
        char c = 0;
	while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, latin_src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unparse_unit_filename(unit, "project.c");
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unparse_unit_filename(unit, "project.c");
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
	while(src_file.get(c)) {
            aunit += c;
        }
        dassert(aunit, src_macro);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        srcml_unparse_unit_filename(unit, "project.c");
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_create_unit(archive);
        dassert(srcml_unparse_unit_filename(unit, "project.c"), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        unit->unit = "<unit/>";
        dassert(srcml_unparse_unit_filename(unit, "project.c"), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unparse_unit_filename(unit, 0), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        dassert(srcml_unparse_unit_filename(0, "project.c"), SRCML_STATUS_ERROR);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    /*
      srcml_unparse_unit_memory
    */

    {

        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unparse_unit_memory(unit, &s, &size);
        dassert(s, src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
        free(s);
    }

    {

        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
	srcml_unit_set_encoding(unit, "UTF-8");
        srcml_unparse_unit_memory(unit, &s, &size);
        dassert(s, utf8_src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
        free(s);
    }

    {

        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unparse_unit_memory(unit, &s, &size);
        dassert(s, latin_src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
        free(s);
    }

    {

        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unparse_unit_memory(unit, &s, &size);
        dassert(s, src_macro);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
        free(s);
    }

    {

        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        srcml_unparse_unit_memory(unit, &s, &size);
        dassert(s, src_macro);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
        free(s);
    }

    {
        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        srcml_unparse_unit_memory(unit, &s, &size);
        dassert(s, src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
        free(s);
    }

    {
        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_create_unit(archive);
        dassert(srcml_unparse_unit_memory(unit, &s, &size), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        unit->unit = "<unit/>";
        dassert(srcml_unparse_unit_memory(unit, &s, &size), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unparse_unit_memory(unit, 0, &size), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        char *s;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unparse_unit_memory(unit, &s, 0), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        char * s;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unparse_unit_memory(0, &s, &size), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    /*
      srcml_unparse_unit_FILE
    */

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unparse_unit_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
	srcml_unit_set_encoding(unit, "UTF-8");
        srcml_unparse_unit_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, utf8_src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unparse_unit_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, latin_src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unparse_unit_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src_macro);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unparse_unit_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src_macro);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        FILE * file = fopen("project.c", "w");
        srcml_unparse_unit_FILE(unit, file);
        fclose(file);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_create_unit(archive);
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unparse_unit_FILE(unit, file), SRCML_STATUS_ERROR);
        fclose(file);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        unit->unit = "<unit/>";
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unparse_unit_FILE(unit, file), SRCML_STATUS_ERROR);
        fclose(file);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unparse_unit_FILE(unit, 0), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        FILE * file = fopen("project.c", "w");
        dassert(srcml_unparse_unit_FILE(0, file), SRCML_STATUS_ERROR);
        fclose(file);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    /*
      srcml_unparse_unit_fd
    */

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        srcml_unparse_unit_fd(unit, fd);
        close(fd);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

	unlink("project.c");

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_utf8.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	srcml_unit_set_encoding(unit, "UTF-8");
        srcml_unparse_unit_fd(unit, fd);
        close(fd);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, utf8_src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

	unlink("project.c");

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_latin.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        srcml_unparse_unit_fd(unit, fd);
        close(fd);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, latin_src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

	unlink("project.c");

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_macro.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        srcml_unparse_unit_fd(unit, fd);
        close(fd);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src_macro);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

	unlink("project.c");

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project_macro_single.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        srcml_unparse_unit_fd(unit, fd);
        close(fd);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src_macro);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

	unlink("project.c");

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit_header(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        srcml_unparse_unit_fd(unit, fd);
        close(fd);
        std::ifstream src_file("project.c");
	std::string aunit;
        char c = 0;
        while(src_file.get(c)) {
            aunit += c;
	}
        dassert(aunit, src);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

	unlink("project.c");

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_create_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        dassert(srcml_unparse_unit_fd(unit, fd), SRCML_STATUS_ERROR);
        close(fd);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        unit->unit = "<unit/>";
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        dassert(srcml_unparse_unit_fd(unit, fd), SRCML_STATUS_ERROR);
        close(fd);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        dassert(srcml_unparse_unit_fd(unit, -1), SRCML_STATUS_ERROR);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_filename(archive, "project.xml");
        srcml_unit * unit = srcml_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        dassert(srcml_unparse_unit_fd(0, fd), SRCML_STATUS_ERROR);
        close(fd);

        srcml_free_unit(unit);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    unlink("project.c");
    unlink("project.xml");
    unlink("project_utf8.cpp");
    unlink("project_utf8.xml");
    unlink("project_latin.cpp");
    unlink("project_latin.xml");
    unlink("project_macro.cpp");
    unlink("project_macro.xml");
    unlink("project_macro_single.xml");

    srcml_cleanup_globals();

    return 0;
}
