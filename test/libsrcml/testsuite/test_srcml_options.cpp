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
        { "//	",  "//      " },
        { "// 	",  "//      " },
        { "//  	",  "//      " },
        { "//   	",  "//      " },
        { "//    	",  "//      " },
        { "//     	",  "//      " },
        { "\"	\"",  "\"       \"" },
        { "\" 	\"",  "\"       \"" },
        { "\"  	\"",  "\"       \"" },
        { "\"   	\"",  "\"       \"" },
        { "\"    	\"",  "\"       \"" },
        { "\"     	\"",  "\"       \"" },
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

    /*
      options attribute
      LINE and NAMESPACEDECL are for internal options, which are not in the result of srcml_archive_get_options()
    */

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options=""><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), 0);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="XMLDECL"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_NO_XML_DECL);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="NAMESPACEDECL"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), 0);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="CPP_TEXT_ELSE"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_CPP_TEXT_ELSE);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="CPP_MARKUP_IF0"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_CPP_MARKUP_IF0);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="LINE"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), 0);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="UNKNOWN"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), 0);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="CPP_TEXT_ELSE,CPP_MARKUP_IF0"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_CPP_TEXT_ELSE | SRCML_OPTION_CPP_MARKUP_IF0));

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="CPP_MARKUP_IF0,CPP_TEXT_ELSE"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_CPP_TEXT_ELSE | SRCML_OPTION_CPP_MARKUP_IF0));

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="LINE,CPP_TEXT_ELSE"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_CPP_TEXT_ELSE);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="CPP_TEXT_ELSE,LINE"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_CPP_TEXT_ELSE);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="UNKNOWN,XMLDECL"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_NO_XML_DECL);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="XMLDECL,NAMESPACEDECL,CPP_TEXT_ELSE,CPP_MARKUP_IF0,LINE"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_NO_XML_DECL | SRCML_OPTION_CPP_TEXT_ELSE | SRCML_OPTION_CPP_MARKUP_IF0));

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options=","><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), 0);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="CPP_TEXT_ELSE,"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_CPP_TEXT_ELSE);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options=",CPP_TEXT_ELSE"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_CPP_TEXT_ELSE);

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string srcml_options = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" language="C++" options="CPP_TEXT_ELSE,,CPP_MARKUP_IF0"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_open_memory(archive, srcml_options.data(), srcml_options.size()), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_CPP_TEXT_ELSE | SRCML_OPTION_CPP_MARKUP_IF0));

        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(!unit, 0);
        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    srcml_cleanup_globals();

    return 0;
}
