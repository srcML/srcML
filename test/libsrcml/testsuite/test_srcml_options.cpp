// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_unit_parse.cpp
 *
 * @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_unit_parse_*()
 */

#include <srcml.h>
#include <dassert.hpp>
#include <string>

struct Pair {
    std::string source;
    std::string expanded;
};

int main(int, char* argv[]) {

    /*
      SRCML_OPTION_EXPAND_TABS
    */

    Pair cases[] = {
        { "	",  "        " },
        { " 	",  "        " },
        { "  	",  "        " },
        { "   	",  "        " },
        { "    	",  "        " },
        { "     	",  "        " },
        { "      	",  "        " },
        { "       	",  "        " },
    };

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {

    	std::cerr << "cases[" << i << "]" << '\n';

        {
            srcml_archive* archive = srcml_archive_create();
            char* xml;
            size_t size;
            srcml_archive_write_open_memory(archive, &xml, &size);
            srcml_unit* unit = srcml_unit_create(archive);
            srcml_unit_set_language(unit, "C++");
            srcml_unit_parse_memory(unit, cases[i].source.data(), cases[i].source.size());

            dassert(srcml_unit_get_src(unit), cases[i].source);

            srcml_unit_free(unit);
            srcml_archive_close(archive);
            srcml_archive_free(archive);
        }

        {
            srcml_archive* archive = srcml_archive_create();
            char* xml;
            size_t size;
            srcml_archive_write_open_memory(archive, &xml, &size);
            srcml_archive_enable_option(archive, SRCML_OPTION_EXPAND_TABS);
            srcml_unit* unit = srcml_unit_create(archive);
            srcml_unit_set_language(unit, "C++");
            srcml_unit_parse_memory(unit, cases[i].source.data(), cases[i].source.size());

            dassert(srcml_unit_get_src(unit), cases[i].expanded);

            srcml_unit_free(unit);
            srcml_archive_close(archive);
            srcml_archive_free(archive);
        }
    }

    srcml_cleanup_globals();

    return 0;
}
