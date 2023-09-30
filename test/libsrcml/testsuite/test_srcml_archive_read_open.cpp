// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_archive_read_open.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_archive_read_open_*
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
    int read_callback(void * context, char* buffer, int len) {
        return (int)fread(buffer, 1, (size_t)len, (FILE*)context);
    }

    int close_callback(void * /* context */) {
        return 0;
    }
}

int main(int, char* argv[]) {
    const std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" url="test" filename="project" version="1">

<unit language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
)";

    const std::string srcml_single = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" url="test" filename="project" version="1"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

    const std::string srcml_ns = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src">

<s:unit language="C++" filename="a.cpp"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>
</s:unit>

<s:unit language="C++" filename="b.cpp"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>

</s:unit>
)";

    std::ofstream srcml_file("project.xml");
    srcml_file << srcml;
    srcml_file.close();

    std::ofstream srcml_file_single("project_single.xml");
    srcml_file_single << srcml_single;
    srcml_file_single.close();

    std::ofstream srcml_file_ns("project_ns.xml");
    srcml_file_ns << srcml_ns;
    srcml_file_ns.close();

    /*
      srcml_archive_read_open_filename
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_filename(archive, "project.xml"), SRCML_STATUS_OK);

        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_filename(archive, "project_single.xml"), SRCML_STATUS_OK);

        dassert(srcml_archive_get_language(archive), 0);
        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_filename(archive, "project_ns.xml"), SRCML_STATUS_OK);

        dassert(srcml_archive_get_namespace_prefix(archive, 0), std::string("s"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_filename(archive, "foobar.xml"), SRCML_STATUS_IO_ERROR);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_filename(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_read_open_filename(0, "project.xml"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_read_open_memory
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml.c_str(), srcml.size()), SRCML_STATUS_OK);

        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_single.c_str(), srcml_single.size()), SRCML_STATUS_OK);

        dassert(srcml_archive_get_language(archive), 0);
        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_ns.c_str(), srcml_ns.size()), SRCML_STATUS_OK);

        dassert(srcml_archive_get_namespace_prefix(archive, 0), std::string("s"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, 0, 1), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml.c_str(), 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_read_open_memory(0, srcml.c_str(), srcml.size()), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_read_open_FILE
    */

    {
        FILE* file = fopen("project.xml", "r");
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_FILE(archive, file), SRCML_STATUS_OK);

        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);
    }

    {
        FILE* file = fopen("project_single.xml", "r");
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_FILE(archive, file), SRCML_STATUS_OK);

        dassert(srcml_archive_get_language(archive), 0);
        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);
    }

    {
        FILE* file = fopen("project_ns.xml", "r");
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_FILE(archive, file), SRCML_STATUS_OK);

        dassert(srcml_archive_get_namespace_prefix(archive, 0), std::string("s"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
    }

    {
        FILE* file = fopen("project_ns.xml", "r");
        dassert(srcml_archive_read_open_FILE(0, file), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);
    }

    /*
      srcml_archive_read_open_fd
    */

    {
        int fd = open("project.xml", O_RDONLY, 0);

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_fd(archive, fd), SRCML_STATUS_OK);

        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        int fd = open("project_single.xml", O_RDONLY, 0);

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_fd(archive, fd), SRCML_STATUS_OK);

        dassert(srcml_archive_get_language(archive), 0);
        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        int fd = open("project_ns.xml", O_RDONLY, 0);

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_fd(archive, fd), SRCML_STATUS_OK);

        dassert(srcml_archive_get_namespace_prefix(archive, 0), std::string("s"));
        dassert(srcml_archive_get_options(archive), 0);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        int fd = open("project_ns.xml", O_RDONLY, 0);
        dassert(srcml_archive_read_open_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
        close(fd);
    }

    /*
      srcml_archive_read_open_io
    */

    {
        FILE* file = fopen("project.xml", "r");

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_io(archive, (void *)file, read_callback, close_callback), SRCML_STATUS_OK);

        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);
    }

    {
        FILE* file = fopen("project_single.xml", "r");

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_io(archive, (void *)file, read_callback, close_callback), SRCML_STATUS_OK);

        dassert(srcml_archive_get_language(archive), 0);
        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_archive_get_version(archive), std::string("1"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);
    }

    {
        FILE* file = fopen("project_ns.xml", "r");

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_io(archive, (void *)file, read_callback, close_callback), SRCML_STATUS_OK);

        dassert(srcml_archive_get_namespace_prefix(archive, 0), std::string("s"));
        dassert(srcml_archive_get_options(archive), 0);

        srcml_archive_close(archive);
        srcml_archive_free(archive);
        fclose(file);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_io(archive, 0, read_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
    }

    {
        FILE* file = fopen("project_ns.xml", "r");
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_io(archive, (void *)file, 0, close_callback), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
        fclose(file);
    }

    {
        FILE* file = fopen("project_ns.xml", "r");
        dassert(srcml_archive_read_open_io(0, (void *)file, read_callback, close_callback), SRCML_STATUS_INVALID_ARGUMENT);
        fclose(file);
    }

    unlink("project.xml");
    unlink("project_single.xml");
    unlink("project_ns.xml");

    srcml_cleanup_globals();

    return 0;
}
