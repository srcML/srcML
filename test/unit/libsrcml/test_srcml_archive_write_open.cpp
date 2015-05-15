/**
 * @file test_srcml_archive_write_open.cpp
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

  Test cases for write open for archives
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

    /*
      srcml_archive_write_open_filename
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_filename(archive, "project.xml", 0);

        dassert(archive->type, SRCML_ARCHIVE_WRITE);
        dassert(!archive->translator, 0);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
                                                     | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_HASH));

        srcml_archive_close(archive);
        srcml_archive_free(archive);

        // check file either does not exist or empty
        std::ifstream result_file("project.xml");
        //assert(!result_file);


    }

    {

        srcml_archive * archive = srcml_archive_create();
        dassert(srcml_archive_write_open_filename(archive, 0, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_write_open_filename(0, "project.xml", 0), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_write_open_memory
    */

    {

        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);

        dassert(archive->type, SRCML_ARCHIVE_WRITE);
        dassert(!archive->translator, 0);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
                                                     | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_HASH));

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        srcml_memory_free(s);

        //dassert(s, 0);
    }

    {

        size_t size;
        srcml_archive * archive = srcml_archive_create();
        dassert(srcml_archive_write_open_memory(archive, 0, &size), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
    }

    {

        char * s = 0;
        srcml_archive * archive = srcml_archive_create();
        dassert(srcml_archive_write_open_memory(archive, &s, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
        srcml_memory_free(s);
    }

    {
        char * s = 0;
        size_t size;
        dassert(srcml_archive_write_open_memory(0, &s, &size), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_memory_free(s);
    }

    /*
      srcml_archive_write_open_FILE
    */

    {
        FILE * file = fopen("project.xml", "w");

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_FILE(archive, file);

        dassert(archive->type, SRCML_ARCHIVE_WRITE);
        dassert(!archive->translator, 0);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
                                                     | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_HASH));

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);

        //char buf[1];
        file = fopen("project.xml", "r");
        //size_t num_read = fread(buf, 1, 1, file);
        fclose(file);
        //dassert(num_read, 0);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        dassert(srcml_archive_write_open_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);

    }

    {
        FILE * file = fopen("project_ns.xml", "w");
        dassert(srcml_archive_write_open_FILE(0, file), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);
    }

    /*
      srcml_archive_write_open_fd
    */

    {
        int fd = OPEN("project.xml", O_WRONLY, 0);

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_fd(archive, fd);

        dassert(archive->type, SRCML_ARCHIVE_WRITE);
        dassert(!archive->translator, 0);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
                                                     | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_HASH));

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        CLOSE(fd);

        //char buf[1];
        //fd = OPEN("project.xml", O_RDONLY, 0);
        //size_t num_read = read(fd, buf, 1);
        CLOSE(fd);
        //dassert(num_read, 0);
    }

    {

        srcml_archive * archive = srcml_archive_create();
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
        FILE * file = fopen("project.xml", "w");

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_io(archive, (void *)file, write_callback, close_callback);

        dassert(archive->type, SRCML_ARCHIVE_WRITE);
        dassert(!archive->translator, 0);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
                                                     | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_HASH));

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);

        //char buf[1];
        file = fopen("project.xml", "r");
        //size_t num_read = fread(buf, 1, 1, file);
        fclose(file);
        //dassert(num_read, 0);

    }

    {

        srcml_archive * archive = srcml_archive_create();
        dassert(srcml_archive_write_open_io(archive, 0, write_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);

    }

    {

        FILE * file = fopen("project_ns.xml", "w");
        srcml_archive * archive = srcml_archive_create();
        dassert(srcml_archive_write_open_io(archive, (void *)file, 0, close_callback), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
        fclose(file);

    }

    {
        FILE * file = fopen("project_ns.xml", "w");
        dassert(srcml_archive_write_open_io(0, (void *)file, write_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);

   }



    UNLINK("project.xml");
    UNLINK("project_ns.xml");

    srcml_cleanup_globals();

    return 0;

}
