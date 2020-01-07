/*
  test_srcml_apply_transforms.cpp

  Copyright (C) 2013-2019 srcML (www.srcML.org)

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/*
  Test cases for srcml_apply_transforms
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

int main(int, char* argv[]) {

    const std::string srcml_a = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";
    const std::string srcml_b = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>
)";
    const std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
)";
    const std::string srcml_a_after = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
)";
    const std::string srcml_b_after = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" version="1">

<s:unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>

</s:unit>
)";
    const std::string srcml_full = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(">

<s:unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>

</s:unit>
)";
    const std::string srcml_full_python = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(">

<s:unit revision=")" SRCML_VERSION_STRING R"(" language="Python" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>

</s:unit>
)";

    const std::string string_xsl = R"(<xsl:stylesheet
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    version="1.0">

<xsl:output method="text"/>
<xsl:template match="/"><xsl:value-of select="'srcML'"/></xsl:template>
</xsl:stylesheet>
)";

    std::ifstream in("copy.xsl");
    std::string copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    std::ifstream inlang("setlanguage.xsl");
    std::string setlanguage((std::istreambuf_iterator<char>(inlang)), std::istreambuf_iterator<char>());

    std::ifstream inschema("schema.rng");
    std::string schema((std::istreambuf_iterator<char>(inschema)), std::istreambuf_iterator<char>());

    //  srcml_apply_transforms

    //  xpath
    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();

        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);

        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_disable_solitary_unit(oarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a_after);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_disable_solitary_unit(oarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b_after);
        free(s);
    }

    //  xpath number result
    {
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xpath(iarchive, "count(//src:unit)");

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NUMBER);
        dassert(srcml_transform_get_number(result), 1.0);
        srcml_transform_free(result);
        srcml_clear_transforms(iarchive);
        srcml_unit_free(unit);

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    //  xpath boolean result
    {
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xpath(iarchive, "count(//src:unit)=1");

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_BOOLEAN);
        dassert(srcml_transform_get_bool(result), true);
        srcml_transform_free(result);
        srcml_clear_transforms(iarchive);
        srcml_unit_free(unit);

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xpath(iarchive, "count(//src:unit)!=1");

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_BOOLEAN);
        dassert(srcml_transform_get_bool(result), false);
        srcml_transform_free(result);
        srcml_clear_transforms(iarchive);
        srcml_unit_free(unit);

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // xpath string
    {
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xpath(iarchive, "string(//src:unit/@language)");

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_STRING);
        dassert(srcml_transform_get_string(result), std::string("C++"));
        srcml_transform_free(result);
        srcml_clear_transforms(iarchive);
        srcml_unit_free(unit);

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    //  xslt_filename

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_filename(iarchive, "setlanguage.xsl");
        srcml_append_transform_param(iarchive, "language", "\"Python\"");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);

        dassert(std::string(s, size), srcml_full_python);
        free(s);
    }

    //  xslt_memory

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_memory(iarchive, setlanguage.c_str(), setlanguage.size());
        srcml_append_transform_param(iarchive, "language", "\"Python\"");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_memory(iarchive, setlanguage.c_str(), setlanguage.size());
        srcml_append_transform_stringparam(iarchive, "language", "Python");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);
    }

    //  xslt_FILE

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        FILE* f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        FILE* f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        FILE* f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        FILE* f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        FILE* f = fopen("setlanguage.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        srcml_append_transform_param(iarchive, "language", "\"Python\"");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);
    }


    //  xslt_fd

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        int fd = OPEN("setlanguage.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);
        srcml_append_transform_param(iarchive, "language", "\"Python\"");

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full_python);
        free(s);
    }

    // xslt string (text)
    {
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_xslt_memory(iarchive, string_xsl.c_str(), string_xsl.size());

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_STRING);
        dassert(srcml_transform_get_string(result), std::string("srcML"));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    //  relaxng_filename

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);
    }


    //  relaxng_memory

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);
    }


    //  relaxng_FILE

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        FILE* f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        FILE* f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        FILE* f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        FILE* f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);
    }

    //  relaxng_fd

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
        int fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b);
        free(s);
    }

    //  all

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml.c_str(), srcml.size());
        srcml_append_transform_xpath(iarchive, "//src:unit");
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        FILE* f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);
        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);

        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());

        srcml_append_transform_xpath(iarchive, "//src:unit");

        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        FILE* f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_full);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());

        srcml_append_transform_xpath(iarchive, "//src:unit");

        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        FILE* f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_disable_solitary_unit(oarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_a_after);
        free(s);
    }

    {
        char* s;
        size_t size;
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());

        srcml_append_transform_xpath(iarchive, "//src:unit");

        srcml_append_transform_xslt_filename(iarchive, "copy.xsl");
        srcml_append_transform_xslt_memory(iarchive, copy.c_str(), copy.size());
        FILE* f = fopen("copy.xsl", "r");
        srcml_append_transform_xslt_FILE(iarchive, f);
        fclose(f);
        int fd = OPEN("copy.xsl", O_RDONLY, 0);
        srcml_append_transform_xslt_fd(iarchive, fd);
        CLOSE(fd);

        srcml_append_transform_relaxng_filename(iarchive, "schema.rng");
        srcml_append_transform_relaxng_memory(iarchive, schema.c_str(), schema.size());
        f = fopen("schema.rng", "r");
        srcml_append_transform_relaxng_FILE(iarchive, f);
        fclose(f);
        fd = OPEN("schema.rng", O_RDONLY, 0);
        srcml_append_transform_relaxng_fd(iarchive, fd);
        CLOSE(fd);

        srcml_archive* oarchive = srcml_archive_clone(iarchive);
        srcml_archive_disable_solitary_unit(oarchive);
        srcml_archive_write_open_memory(oarchive, &s, &size);

        srcml_unit* unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(!srcml_transform_get_unit(result, 0), false);

        srcml_archive_write_unit(oarchive, srcml_transform_get_unit(result, 0));
        srcml_transform_free(result);
        srcml_unit_free(unit);
        srcml_clear_transforms(iarchive);

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        dassert(std::string(s, size), srcml_b_after);
        free(s);
    }

    srcml_cleanup_globals();

    return 0;
}
