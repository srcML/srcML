// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_archive_create.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_archive_create()
 */

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_language(archive), 0);
        dassert(srcml_archive_get_xml_encoding(archive), 0);
        dassert(srcml_archive_get_src_encoding(archive), 0);
        dassert(srcml_archive_get_url(archive), 0);
        dassert(srcml_archive_get_version(archive), 0);
        dassert(srcml_archive_get_tabstop(archive), 8);
        dassert(srcml_archive_get_namespace_size(archive), 1);
        dassert(srcml_archive_get_namespace_uri(archive, 0), std::string("http://www.srcML.org/srcML/src"));
        srcml_archive_free(archive);
    }

    return 0;
}
