/**
 * @file test_srcml_clear_transforms.cpp
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

  Test cases for srcml_clear_transforms
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

    /*
      srcml_clear_transforms
    */

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        dassert(archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        srcml_append_transform_xpath(archive, "//src:unit");
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }


    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        srcml_append_transform_xslt_filename(archive, "copy.xsl");
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size());
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(archive, f);
        fclose(f);
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(archive, fd);
        CLOSE(fd);
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        srcml_append_transform_relaxng_filename(archive, "schema.rng");
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size());
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        FILE * f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(archive, f);
        fclose(f);
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(archive, fd);
        CLOSE(fd);
        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        archive->type = SRCML_ARCHIVE_RW;

        srcml_append_transform_xpath(archive, "//src:unit");

        srcml_append_transform_xslt_filename(archive, "copy.xsl");
        srcml_append_transform_xslt_memory(archive, copy.c_str(), copy.size());
        FILE * f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(archive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(archive, fd);
        CLOSE(fd);

        srcml_append_transform_relaxng_filename(archive, "schema.rng");
        srcml_append_transform_relaxng_memory(archive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(archive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(archive, fd);
        CLOSE(fd);

        dassert(!archive->transformations.size(), 0);
        dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
        dassert(archive->transformations.size(), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_clear_transforms(0), SRCML_STATUS_INVALID_ARGUMENT);
    }

    srcml_cleanup_globals();

    return 0;

}
