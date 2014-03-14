/**
 * @file test_srcml_append_transform.cpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <string>
#include <fstream>
#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include "dassert.hpp"

int main() {

    std::string copy;
    {
	std::ifstream in("copy.xsl");
	char c = 0;
        while(in.get(c)) {
            copy += c;
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

    std::string s = "<unit/>";

    /*
      srcml_append_transform_xpath
    */

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        srcml_append_transform_xpath(archive, "//src:unit");

        dassert(archive->transformations.back().type, SRCML_XPATH);
        dassert(archive->transformations.back().transformation.str, "//src:unit");

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        dassert(srcml_append_transform_xpath(archive, "//src:unit"), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_xpath(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        dassert(srcml_append_transform_xpath(0, "//src:unit"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xslt_filename
    */

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        srcml_append_transform_xslt_filename(archive, "copy.xsl");

        dassert(archive->transformations.back().type, SRCML_XSLT);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        dassert(srcml_append_transform_xslt_filename(archive, "copy.xsl"), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_xslt_filename(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        dassert(srcml_append_transform_xslt_filename(0, "copy.xsl"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xslt_memory
    */

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size());

        dassert(archive->transformations.back().type, SRCML_XSLT);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size()), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_xslt_memory(archive, 0, copy.size()), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_xslt_memory(archive, copy.c_str(), 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        dassert(srcml_append_transform_xslt_memory(0, copy.c_str(), copy.size()), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_xslt_FILE
    */

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
	FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(archive, f);
	fclose(f);

        dassert(archive->transformations.back().type, SRCML_XSLT);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
	FILE * f = fopen("copy.xsl", "r");
        dassert(srcml_append_transform_xslt_FILE(archive, f), SRCML_STATUS_INVALID_IO_OPERATION);
        fclose(f);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_xslt_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
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
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        int fd = open("copy.xsl", O_RDONLY);
        srcml_append_transform_xslt_fd(archive, fd);
	close(fd);

        dassert(archive->transformations.back().type, SRCML_XSLT);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        int fd = open("copy.xsl", O_RDONLY);
        dassert(srcml_append_transform_xslt_fd(archive, fd), SRCML_STATUS_INVALID_IO_OPERATION);
	close(fd);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_xslt_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        int fd = open("copy.xsl", O_RDONLY);
        dassert(srcml_append_transform_xslt_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
	close(fd);
    }

    /*
      srcml_append_transform_relaxng_filename
    */

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        srcml_append_transform_relaxng_filename(archive, "schema.rng");

        dassert(archive->transformations.back().type, SRCML_RELAXNG);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        dassert(srcml_append_transform_relaxng_filename(archive, "schema.rng"), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_relaxng_filename(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        dassert(srcml_append_transform_relaxng_filename(0, "schema.rng"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_relaxng_memory
    */

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size());

        dassert(archive->transformations.back().type, SRCML_RELAXNG);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size()), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_relaxng_memory(archive, 0, schema.size()), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_relaxng_memory(archive, schema.c_str(), 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        dassert(srcml_append_transform_relaxng_memory(0, schema.c_str(), schema.size()), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_append_transform_relaxng_FILE
    */

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
	FILE * f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(archive, f);
	fclose(f);

        dassert(archive->transformations.back().type, SRCML_RELAXNG);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
	FILE * f = fopen("schema.rng", "r");
        dassert(srcml_append_transform_relaxng_FILE(archive, f), SRCML_STATUS_INVALID_IO_OPERATION);
        fclose(f);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_relaxng_FILE(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
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
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
	int fd = open("schema.rng", O_RDONLY);
        srcml_append_transform_relaxng_fd(archive, fd);
	close(fd);

        dassert(archive->transformations.back().type, SRCML_RELAXNG);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
	int fd = open("schema.rng", O_RDONLY);
        dassert(srcml_append_transform_relaxng_fd(archive, fd), SRCML_STATUS_INVALID_IO_OPERATION);
	close(fd);

        srcml_free_archive(archive);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());
        dassert(srcml_append_transform_relaxng_fd(archive, -1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    {

	int fd = open("schema.rng", O_RDONLY);
        dassert(srcml_append_transform_relaxng_fd(0, fd), SRCML_STATUS_INVALID_ARGUMENT);
	close(fd);
    }

    /*
      srcml_append_transform_*
    */

    {
        srcml_archive * archive = srcml_create_archive();
        srcml_read_open_memory(archive, s.c_str(), s.size());

        srcml_append_transform_xpath(archive, "//src:unit");

        srcml_append_transform_xslt_filename(archive, "copy.xsl");
        srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size());
	FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(archive, f);
	fclose(f);
	int fd = open("copy.xsl", O_RDONLY);
        srcml_append_transform_xslt_fd(archive, fd);
	close(fd);

        srcml_append_transform_relaxng_filename(archive, "schema.rng");
        srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size());
	f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(archive, f);
	fclose(f);
	fd = open("copy.xsl", O_RDONLY);
        srcml_append_transform_relaxng_fd(archive, fd);
	close(fd);

        dassert(archive->transformations.at(0).type, SRCML_XPATH);
        dassert(archive->transformations.at(0).transformation.str, "//src:unit");
        dassert(archive->transformations.at(1).type, SRCML_XSLT);
        assert(archive->transformations.at(1).transformation.doc != 0);
        dassert(archive->transformations.at(1).type, SRCML_XSLT);
        assert(archive->transformations.at(1).transformation.doc != 0);
        dassert(archive->transformations.at(1).type, SRCML_XSLT);
        assert(archive->transformations.at(1).transformation.doc != 0);
        dassert(archive->transformations.at(1).type, SRCML_XSLT);
        assert(archive->transformations.at(1).transformation.doc != 0);
        dassert(archive->transformations.back().type, SRCML_RELAXNG);
        assert(archive->transformations.back().transformation.doc != 0);
        dassert(archive->transformations.back().type, SRCML_RELAXNG);
        assert(archive->transformations.back().transformation.doc != 0);
        dassert(archive->transformations.back().type, SRCML_RELAXNG);
        assert(archive->transformations.back().transformation.doc != 0);
        dassert(archive->transformations.back().type, SRCML_RELAXNG);
        assert(archive->transformations.back().transformation.doc != 0);

        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    srcml_cleanup_globals();

    return 0;

}
