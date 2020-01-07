/**
 * @file test_srcml_archive_write_open.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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

  Test cases for write open for archives
*/

#include <srcml.h>

#include <macros.hpp>

#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <dassert.hpp>

int write_callback(void * context, const char * buffer, int len) {
    return (int)fwrite(buffer, 1, len, (FILE*)context);

}

int close_callback(void * context UNUSED) {
    return 0;
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
        size_t size = -1;
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_memory(archive, &s, &size), SRCML_STATUS_OK);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        size_t size = -1;
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_memory(archive, 0, &size), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(size, -1);
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
        size_t size = -1;
        dassert(srcml_archive_write_open_memory(0, &s, &size), SRCML_STATUS_INVALID_ARGUMENT);
        dassert(size, -1);
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
        int fd = OPEN("project.xml", O_WRONLY, 0);
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_fd(archive, fd), SRCML_STATUS_OK);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        CLOSE(fd);

        char buf[1];
        fd = OPEN("project.xml", O_RDONLY, 0);
        size_t num_read = read(fd, buf, 1);
        CLOSE(fd);

        dassert(num_read, 1);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_write_open_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        int fd = OPEN("project_ns.xml", O_WRONLY, 0);
        dassert(srcml_archive_write_open_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
        CLOSE(fd);
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

    UNLINK("project.xml");
    UNLINK("project_ns.xml");

    srcml_cleanup_globals();

    return 0;
}
