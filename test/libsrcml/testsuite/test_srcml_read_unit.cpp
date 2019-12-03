/**
 * @file test_srcml_archive_read_unit.cpp
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

  Test cases for srcml_archive_read_unit
*/

#include <srcml.h>

#include <fstream>
#include <fcntl.h>

#include <dassert.hpp>

int main(int, char* argv[]) {
    const std::string srcml_a = R"(<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_b = R"(<s:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>)";

    const std::string srcml_b_single = R"(<s:unit xmlns:s="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++" url="test" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>)";

    const std::string srcml_b_two = R"(<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C" filename="project.c"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_timestamp_inner = R"(<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" timestamp="today" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_timestamp_single_inner = R"(<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" timestamp="today" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_hash_inner = R"(<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_hash_single_inner = R"(<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src">

<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
)";

    const std::string srcml_soleunit = R"(<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_full = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" url="test">

<s:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>

</s:unit>
)";
    const std::string srcml_single = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++" url="test" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>
)";

    const std::string srcml_two = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src">

<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C" filename="project.c"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
)";

    const std::string srcml_timestamp = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src">

<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" timestamp="today" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
)";
    const std::string srcml_timestamp_single = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" timestamp="today" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

    const std::string srcml_hash = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src">

<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
)";
    const std::string srcml_hash_single = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb" language="C" filename="project.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

    /*
      srcml_archive_read_unit
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml.c_str(), srcml.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_srcml_outer(unit), srcml_a);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_full.c_str(), srcml_full.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C++"));
        dassert(srcml_unit_get_filename(unit), std::string("project"));
        dassert(srcml_unit_get_version(unit), std::string("1"));
        dassert(srcml_unit_get_srcml_outer(unit), srcml_b);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C++"));
        dassert(srcml_unit_get_filename(unit), std::string("project"));
        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_unit_get_version(unit), std::string("1"));
        dassert(srcml_unit_get_srcml(unit), srcml_b_single);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_two.c_str(), srcml_two.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C"));
        dassert(srcml_unit_get_filename(unit), std::string("project.c"));
        dassert(srcml_archive_get_url(archive), 0);
        dassert(srcml_unit_get_version(unit), 0);
        dassert(srcml_unit_get_srcml_outer(unit), srcml_a);
        srcml_unit_free(unit);
        unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C"));
        dassert(srcml_unit_get_filename(unit), std::string("project.c"));
        dassert(srcml_archive_get_url(archive), 0);
        dassert(srcml_unit_get_version(unit), 0);
        dassert(srcml_unit_get_srcml_outer(unit), srcml_b_two);
        srcml_unit_free(unit);
        unit = srcml_archive_read_unit(archive);
        dassert(unit, 0);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_timestamp.c_str(), srcml_timestamp.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_timestamp(unit), std::string("today"));
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_timestamp_single.c_str(), srcml_timestamp_single.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_timestamp(unit), std::string("today"));
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_hash.c_str(), srcml_hash.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_hash(unit), std::string("aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"));
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_hash_single.c_str(), srcml_hash_single.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_hash(unit), std::string("aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"));
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_unit(archive), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_read_unit(0), 0);
    }

    /*
      srcml_archive_read_unit
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml.c_str(), srcml.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C"));
        dassert(srcml_unit_get_filename(unit), std::string("project.c"));
        dassert(srcml_archive_get_url(archive), 0);
        dassert(srcml_unit_get_version(unit), 0);
        dassert(srcml_unit_get_srcml_outer(unit), std::string(srcml_soleunit));
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_full.c_str(), srcml_full.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C++"));
        dassert(srcml_unit_get_filename(unit), std::string("project"));
        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_unit_get_version(unit), std::string("1"));
        dassert(srcml_unit_get_srcml_inner(unit), std::string("<s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n"));
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C++"));
        dassert(srcml_unit_get_filename(unit), std::string("project"));
        dassert(srcml_archive_get_url(archive), std::string("test"));
        dassert(srcml_unit_get_version(unit), std::string("1"));
        dassert(srcml_unit_get_srcml_inner(unit), std::string("<s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n"));
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, srcml_two.c_str(), srcml_two.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C"));
        dassert(srcml_unit_get_filename(unit), std::string("project.c"));
        dassert(srcml_archive_get_url(archive), 0);
        dassert(srcml_unit_get_version(unit), 0);
        dassert(srcml_unit_get_srcml_inner(unit), std::string("<expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n"));
        srcml_unit_free(unit);
        unit = srcml_archive_read_unit(archive);
        dassert(srcml_unit_get_language(unit), std::string("C"));
        dassert(srcml_unit_get_filename(unit), std::string("project.c"));
        dassert(srcml_archive_get_url(archive), 0);
        dassert(srcml_unit_get_version(unit), 0);
        dassert(srcml_unit_get_srcml_inner(unit), std::string("<expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n"));
        srcml_unit_free(unit);
        unit = srcml_archive_read_unit(archive);
        dassert(unit, 0);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_read_unit(archive), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_read_unit(0), 0);
    }

    srcml_cleanup_globals();

    return 0;
}
