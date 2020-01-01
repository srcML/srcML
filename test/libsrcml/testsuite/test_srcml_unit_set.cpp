/**
 * @file test_srcml_unit_set.cpp
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

  Test cases for srcml_unit_set_*.
*/

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    srcml_archive* archive = srcml_archive_create();

    /*
      srcml_unit_set_src_encoding
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, 0);

        dassert(srcml_unit_get_src_encoding(unit), 0);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_get_src_encoding(unit), std::string("ISO-8859-1"));

        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_src_encoding(0, "ISO-8859-1"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_language
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, 0);

        dassert(srcml_unit_get_language(unit), 0);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");

        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_language(0, "C++"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_filename
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, 0);

        dassert(srcml_unit_get_filename(unit), 0);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "main.cpp");

        dassert(srcml_unit_get_filename(unit), std::string("main.cpp"));

        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_filename(0, "main.cpp"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_version
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, 0);

        dassert(srcml_unit_get_version(unit), 0);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, "0.9.5");

        dassert(srcml_unit_get_version(unit), std::string("0.9.5"));

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit1 = srcml_unit_create(archive);

        dassert(srcml_unit_set_version(unit1, "v1"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_version(unit1), std::string("v1"));

        srcml_unit* unit2 = srcml_unit_create(archive);

        dassert(srcml_unit_set_version(unit2, "v2"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_version(unit2), std::string("v2"));

        srcml_archive_set_version(archive, "archiveVersion");

        dassert(srcml_unit_get_version(unit1), std::string("v1"));
        dassert(srcml_unit_get_version(unit2), std::string("v2"));

        srcml_unit_free(unit1);
        srcml_unit_free(unit2);
    }

    {
        dassert(srcml_unit_set_version(0, "v1"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_timestamp
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, 0);

        dassert(srcml_unit_get_timestamp(unit), 0);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, "Wed Jul  3 16:38:14 EDT 2019");

        dassert(srcml_unit_get_timestamp(unit), std::string("Wed Jul  3 16:38:14 EDT 2019"));

        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_timestamp(0, "Wed Jul  3 16:38:14 EDT 2019"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_eol
    */

    {
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_AUTO), SRCML_STATUS_OK);
        dassert(srcml_unit_get_eol(unit), SOURCE_OUTPUT_EOL_AUTO);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_LF), SRCML_STATUS_OK);
        dassert(srcml_unit_get_eol(unit), SOURCE_OUTPUT_EOL_LF);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CR), SRCML_STATUS_OK);
        dassert(srcml_unit_get_eol(unit), SOURCE_OUTPUT_EOL_CR);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CRLF), SRCML_STATUS_OK);
        dassert(srcml_unit_get_eol(unit), SOURCE_OUTPUT_EOL_CRLF);

        srcml_unit_free(unit);
    }

    {
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CRLF + 1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
    }

    {
        dassert(srcml_unit_set_eol(0, SOURCE_OUTPUT_EOL_AUTO), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        std::string text = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, text.c_str(), text.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);

        char* buf;
        size_t size;

        srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_LF);
        srcml_unit_unparse_memory(unit, &buf, &size);
        dassert(buf, std::string("\na;\n"));
        free(buf);

        srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CRLF);
        srcml_unit_unparse_memory(unit, &buf, &size);
        dassert(buf, std::string("\r\na;\r\n"));
        free(buf);

        srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CR);
        srcml_unit_unparse_memory(unit, &buf, &size);
        dassert(buf, std::string("\ra;\r"));
        free(buf);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    srcml_archive_free(archive);

    return 0;
}
