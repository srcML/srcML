// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_unit_unparse.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_unit_unparse
 */

#include <srcml.h>
#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#ifdef _MSC_VER
#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE
#endif

#include <dassert.hpp>

extern "C" {
    int write_callback(void* context, const char* buffer, int len) {

        return (int)fwrite(buffer, 1, (size_t)len, (FILE*)context);
    }

    int close_callback(void* /* context */) {

        return 0;
    }
}

int main(int, char* argv[]) {

    const std::string src = "a;\n";
    const std::string utf8_src = "/* ✓ */\n";
    const std::string latin_src = "/* &#10003; */\n";

    std::ofstream srcml_file("project.xml");
    srcml_file << R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src">

<unit language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
)";
    srcml_file.close();

    std::ofstream srcml_utf8_file("project_utf8.xml");
    srcml_utf8_file << R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" url="test" filename="project" version="1"><comment type="block">/* ✓ */</comment>
</unit>
)";
    srcml_utf8_file.close();

    std::ofstream srcml_latin_file("project_latin.xml");
    srcml_latin_file << R"(<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" url="test" filename="project" version="1"><comment type="block">/* &#10003; */</comment>
</unit>
)";
    srcml_latin_file.close();

    std::ofstream srcml_latin_from_utf8_file("project_latin_from_utf8.xml");
    srcml_latin_from_utf8_file << R"(<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" url="test" filename="project" version="1"><comment type="block">/* &#10003; */</comment>
</unit>
)";
    srcml_latin_from_utf8_file.close();

    std::ofstream srcml_latin_from_latin_file("project_latin_from_latin.xml");
    srcml_latin_from_latin_file << R"(<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" url="test" filename="project" version="1"><comment type="block">/* &#10003; */</comment>
</unit>
)";
    srcml_latin_from_latin_file.close();

    /*
      srcml_unit_unparse_filename
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_filename(unit, "project.c"), SRCML_STATUS_OK);
        std::ifstream src_file("project.c");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_filename(unit, "project_utf8.cpp"), SRCML_STATUS_OK);
        std::ifstream src_file("project_utf8.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_filename(unit, "project_latin_from_utf8.cpp"), SRCML_STATUS_OK);
        std::ifstream src_file("project_latin_from_utf8.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_filename(unit, "project_latin.cpp"), SRCML_STATUS_OK);
        std::ifstream src_file("project_latin.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_filename(unit, "project_latin_from_latin.cpp"), SRCML_STATUS_OK);
        std::ifstream src_file("project_latin_from_latin.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_filename(unit, "project.c"), SRCML_STATUS_OK);
        std::ifstream src_file("project.c");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_unparse_filename(unit, "project.c"), SRCML_STATUS_UNINITIALIZED_UNIT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_unparse_filename(unit, "project.c"),  SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_filename(unit, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");

        dassert(srcml_unit_unparse_filename(0, "project.c"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      srcml_unit_unparse_memory
    */

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_OK);
        dassert(s, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_OK);
        dassert(s, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_OK);
        dassert(s, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_OK);
        dassert(s, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_OK);
        dassert(s, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_OK);
        dassert(s, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_UNINITIALIZED_UNIT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_unparse_memory(unit, &s, &size), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_memory(unit, 0, &size), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        char *s;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_memory(unit, &s, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        char* s;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_memory(0, &s, &size), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      srcml_unit_unparse_FILE
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project.c");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project_utf8.cpp", "w");
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project_utf8.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_utf8.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project_latin_from_utf8.cpp", "w");
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project_latin_from_utf8.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin_from_utf8.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project_latin.cpp", "w");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project_latin.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project_latin_from_latin.cpp", "w");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project_latin_from_latin.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin_from_latin.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project.c");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_UNINITIALIZED_UNIT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_FILE(unit, file), SRCML_STATUS_INVALID_IO_OPERATION);

        fclose(file);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_FILE(unit, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_FILE(0, file), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    /*
      srcml_unit_unparse_fd
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);

        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_OK);
        close(fd);
        std::ifstream src_file("project.c");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        int fd = open("project_utf8.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_OK);
        close(fd);
        std::ifstream src_file("project_utf8.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_utf8.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        int fd = open("project_latin_from_utf8.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_OK);
        close(fd);
        std::ifstream src_file("project_latin_from_utf8.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin_from_utf8.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        int fd = open("project_latin.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_OK);
        close(fd);
        std::ifstream src_file("project_latin.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        int fd = open("project_latin_from_latin.cpp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_OK);
        close(fd);
        std::ifstream src_file("project_latin_from_latin.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin_from_latin.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);

        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_OK);
        close(fd);
        std::ifstream src_file("project.c");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);

        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_UNINITIALIZED_UNIT);
        close(fd);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);

        dassert(srcml_unit_unparse_fd(unit, fd), SRCML_STATUS_INVALID_IO_OPERATION);

        close(fd);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_fd(unit, -1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        int fd = open("project.c", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);

        dassert(srcml_unit_unparse_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
        close(fd);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    /*
      srcml_unit_unparse_io
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project.c");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project_utf8.cpp", "w");
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project_utf8.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_utf8.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_utf8.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project_latin_from_utf8.cpp", "w");
        srcml_unit_set_src_encoding(unit, "UTF-8");

        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project_latin_from_utf8.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, utf8_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin_from_utf8.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project_latin.cpp", "w");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project_latin.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project_latin_from_latin.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project_latin_from_latin.cpp", "w");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project_latin_from_latin.cpp");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, latin_src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project_latin_from_latin.cpp");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_OK);
        fclose(file);
        std::ifstream src_file("project.c");
        std::string aunit((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());
        dassert(aunit, src);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_UNINITIALIZED_UNIT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_io(unit, (void *)file, write_callback, close_callback), SRCML_STATUS_INVALID_IO_OPERATION);

        fclose(file);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);

        dassert(srcml_unit_unparse_io(unit, 0, write_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_io(unit, (void *)file, 0, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_archive_read_unit(archive);
        FILE* file = fopen("project.c", "w");

        dassert(srcml_unit_unparse_io(0, (void *)file, write_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        unlink("project.c");
    }

    unlink("project.xml");
    unlink("project_utf8.xml");
    unlink("project_latin.xml");
    unlink("project_latin_from_utf8.xml");
    unlink("project_latin_from_latin.xml");

    srcml_cleanup_globals();

    return 0;
}
