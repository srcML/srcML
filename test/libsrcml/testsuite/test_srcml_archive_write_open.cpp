// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_archive_write_open.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_archive_write_open_*
 */

#include <srcml.h>

#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <dassert.hpp>

extern "C" {
    int write_callback(void * context, const char * buffer, int len) {
        return (int)fwrite(buffer, 1, (size_t)len, (FILE*)context);

    }

    int close_callback(void* /* context */) {
        return 0;
    }
}

int main(int, char* argv[]) {

    /*
      srcml_archive_write_open_filename
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_filename(archive, "project.xml"), SRCML_STATUS_OK);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        // check file either does not exist or empty
        std::ifstream result_file("project.xml");
        dassert(bool(result_file), true);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_filename(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_write_open_filename(0, "project.xml"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_write_open_memory
    */

    {
        char* s = 0;
        size_t size = 0;
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_memory(archive, &s, &size), SRCML_STATUS_OK);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        size_t size = 0;
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_memory(archive, 0, &size), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(size, 0);
    }

    {
        char* s = 0;
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_memory(archive, &s, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char* s = 0;
        size_t size = 0;
        dassert(srcml_archive_write_open_memory(0, &s, &size), SRCML_STATUS_INVALID_ARGUMENT);
        dassert(size, 0);
    }

    /*
      srcml_archive_write_open_FILE
    */

    {
        FILE* file = fopen("project.xml", "w");
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_FILE(archive, file), SRCML_STATUS_OK);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);

        char buf[1];
        file = fopen("project.xml", "r");
        size_t num_read = fread(buf, 1, 1, file);
        fclose(file);

        dassert(num_read, 1);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        FILE* file = fopen("project_ns.xml", "w");
        dassert(srcml_archive_write_open_FILE(0, file), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);
    }

    /*
      srcml_archive_write_open_fd
    */

    {
        int fd = open("project.xml", O_WRONLY, 0);
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_fd(archive, fd), SRCML_STATUS_OK);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        close(fd);

        char buf[1];
        fd = open("project.xml", O_RDONLY, 0);
        ssize_t num_read = read(fd, buf, 1);
        close(fd);

        dassert(num_read, 1);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        int fd = open("project_ns.xml", O_WRONLY, 0);
        dassert(srcml_archive_write_open_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
        close(fd);
    }

    /*
      srcml_archive_write_open_io
    */

    {
        FILE* file = fopen("project.xml", "w");
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_io(archive, (void *)file, write_callback, close_callback), SRCML_STATUS_OK);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);

        char buf[1];
        file = fopen("project.xml", "r");
        size_t num_read = fread(buf, 1, 1, file);
        fclose(file);

        dassert(num_read, 1);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_io(archive, 0, write_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        FILE* file = fopen("project_ns.xml", "w");
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_io(archive, (void *)file, 0, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
        fclose(file);
    }

    {
        FILE* file = fopen("project_ns.xml", "w");
        dassert(srcml_archive_write_open_io(0, (void *)file, write_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);
   }

    unlink("project.xml");
    unlink("project_ns.xml");

    srcml_cleanup_globals();

    return 0;
}
