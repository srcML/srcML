// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcql.cpp
 *
 * @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_append_transform_srcql*()
*/

#include <srcml.h>

#include <iostream>
#include <string>
#include <vector>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    const std::string int_x_decl_srcml = 
        R"(<decl><type><name>int</name></type> <name>x</name></decl>)";

    const std::string int_x_decl_stmt_srcml = 
        R"(<decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt>)";

    const std::string include_iostream_srcml = 
        R"(<cpp:include>#<cpp:directive>include</cpp:directive> <cpp:file>&lt;iostream&gt;</cpp:file></cpp:include>)";

    const std::string include_string_srcml = 
        R"(<cpp:include>#<cpp:directive>include</cpp:directive> <cpp:file>&lt;string&gt;</cpp:file></cpp:include>)";

    const std::string int_x_decl_statement_src = R"(
int x;
)";

    const std::string int_x_function_decl_statement_src = R"(
void foo() {
    int x;
}
)";

    const std::string int_x_parameter_src = R"(
void foo(int x) {}
)";

    const std::string include_iostream_src = R"(
#include <iostream>
)";

    const std::string include_string_src = R"(
#include <string>
)";

    ////// Raw Source Code Expressions

    //// srcML Tags
    // src:decl
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,"",0);
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_decl_statement_src.c_str(),int_x_decl_statement_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_function_decl_statement_src.c_str(),int_x_function_decl_statement_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_parameter_src.c_str(),int_x_parameter_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        std::vector<std::string> unit_strings {int_x_decl_statement_src,int_x_function_decl_statement_src,int_x_parameter_src};
        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,"C++");
            srcml_unit_parse_memory(unit,unit_strings[i].c_str(),unit_strings[i].size());
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);
            srcml_unit_free(unit);
        }

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl"), SRCML_STATUS_OK);

        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            srcml_unit_apply_transforms(iarchive, unit, &result);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
            dassert(srcml_transform_get_unit_size(result), 1);
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);

            srcml_unit_free(unit);
            srcml_transform_free(result);
        }
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    // src:decl_stmt
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_decl_statement_src.c_str(),int_x_decl_statement_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_function_decl_statement_src.c_str(),int_x_function_decl_statement_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_parameter_src.c_str(),int_x_parameter_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_stmt_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        std::vector<std::string> unit_strings {int_x_decl_statement_src,int_x_function_decl_statement_src,int_x_parameter_src};
        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,"C++");
            srcml_unit_parse_memory(unit,unit_strings[i].c_str(),unit_strings[i].size());
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);
            srcml_unit_free(unit);
        }

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:decl_stmt"), SRCML_STATUS_OK);

        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            srcml_unit_apply_transforms(iarchive, unit, &result);

            if (i < 2) {
                dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
                dassert(srcml_transform_get_unit_size(result), 1);
                dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);
            }
            else {
                dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);
            }

            srcml_unit_free(unit);
            srcml_transform_free(result);
        }
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // cpp:include
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,include_iostream_src.c_str(),include_iostream_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_iostream_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,include_string_src.c_str(),include_string_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_string_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = include_iostream_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_iostream_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = include_string_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_string_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = include_iostream_src + include_string_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_iostream_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), include_string_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }



    //// XPath
    // //src:decl
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,"",0);
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_decl_statement_src.c_str(),int_x_decl_statement_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_function_decl_statement_src.c_str(),int_x_function_decl_statement_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_parameter_src.c_str(),int_x_parameter_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        std::vector<std::string> unit_strings {int_x_decl_statement_src,int_x_function_decl_statement_src,int_x_parameter_src};
        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,"C++");
            srcml_unit_parse_memory(unit,unit_strings[i].c_str(),unit_strings[i].size());
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);
            srcml_unit_free(unit);
        }

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl"), SRCML_STATUS_OK);

        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            srcml_unit_apply_transforms(iarchive, unit, &result);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
            dassert(srcml_transform_get_unit_size(result), 1);
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);

            srcml_unit_free(unit);
            srcml_transform_free(result);
        }
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    // //src:decl_stmt
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_decl_statement_src.c_str(),int_x_decl_statement_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_function_decl_statement_src.c_str(),int_x_function_decl_statement_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_parameter_src.c_str(),int_x_parameter_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_stmt_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        std::vector<std::string> unit_strings {int_x_decl_statement_src,int_x_function_decl_statement_src,int_x_parameter_src};
        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,"C++");
            srcml_unit_parse_memory(unit,unit_strings[i].c_str(),unit_strings[i].size());
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);
            srcml_unit_free(unit);
        }

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:decl_stmt"), SRCML_STATUS_OK);

        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            srcml_unit_apply_transforms(iarchive, unit, &result);

            if (i < 2) {
                dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
                dassert(srcml_transform_get_unit_size(result), 1);
                dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);
            }
            else {
                dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);
            }

            srcml_unit_free(unit);
            srcml_transform_free(result);
        }
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // //cpp:include
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,include_iostream_src.c_str(),include_iostream_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_iostream_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,include_string_src.c_str(),include_string_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_string_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = include_iostream_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_iostream_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = include_string_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_string_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = include_iostream_src + include_string_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_iostream_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), include_string_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string name_functions_src = R"(
void foo() {}

int bar() {}

bool baz() {}
)";

    // //src:function/src:name
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_functions_src.c_str(),name_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:function/src:name"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>foo</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>bar</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>baz</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // //src:type/src:name
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_functions_src.c_str(),name_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"//src:type/src:name"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>void</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>bool</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // //src:function[src:type[src:name[.="void"]]]
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_functions_src.c_str(),name_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(//src:function[src:type[src:name[.="void"]]])"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<function><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    //// Source Code Patterns
    // int $T
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src + int_x_function_decl_statement_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"int $N"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $T x
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src + int_x_function_decl_statement_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$T x"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string int_y_decl_srcml = 
        R"(<decl><type><name>int</name></type> <name>y</name></decl>)";

    const std::string int_y_decl_stmt_srcml = 
        R"(<decl_stmt><decl><type><name>int</name></type> <name>y</name></decl>;</decl_stmt>)";

    const std::string int_y_decl_statement_src = R"(
int y;
)";

    const std::string int_y_function_decl_statement_src = R"(
int foo() {
    int y;
}
)";

    const std::string int_y_parameter_src = R"(
int foo(int y) {}
)";

    // $T y
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_y_decl_statement_src + int_y_function_decl_statement_src + int_y_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$T y"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_y_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_y_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_y_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $T $N
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src + int_x_function_decl_statement_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$T $N"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_y_decl_statement_src + int_y_function_decl_statement_src + int_y_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$T $N"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_y_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_y_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_y_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src + int_y_decl_statement_src + int_x_function_decl_statement_src + int_y_function_decl_statement_src + int_x_parameter_src + int_y_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$T $N"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 6);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_y_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), int_y_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), int_y_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string list_of_exprs = R"(
/*0*/   1;
/*1*/   2;
/*2*/   x;
/*3*/   y;
/*4*/   z;
/*5*/   1 + 1;
/*6*/   1 + 2;
/*7*/   2 + 1;
/*8*/   x + x;
/*9*/   x + y;
/*10*/  x + z;
/*11*/  y + x;
/*12*/  y + y;
/*13*/  y + z;
/*14*/  z + x;
/*15*/  z + y;
/*16*/  z + z;
/*17*/  x + x + x;
/*18*/  x + x + y;
/*19*/  x + x + z;
/*20*/  x + y + x;
/*21*/  x + y + y;
/*22*/  x + y + z;
/*23*/  x + z + x;
/*24*/  x + z + y;
/*25*/  x + z + z;
/*26*/  y + x + x;
/*27*/  y + x + y;
/*28*/  y + x + z;
/*29*/  y + y + x;
/*30*/  y + y + y;
/*31*/  y + y + z;
/*32*/  y + z + x;
/*33*/  y + z + y;
/*34*/  y + z + z;
/*35*/  z + x + x;
/*36*/  z + x + y;
/*37*/  z + x + z;
/*38*/  z + y + x;
/*39*/  z + y + y;
/*40*/  z + y + z;
/*41*/  z + z + x;
/*42*/  z + z + y;
/*43*/  z + z + z;
)";

    const std::vector<std::string> exprs_srcml {
/*0*/   R"(<expr><literal type="number">1</literal></expr>)",
/*1*/   R"(<expr><literal type="number">2</literal></expr>)",
/*2*/   R"(<expr><name>x</name></expr>)",
/*3*/   R"(<expr><name>y</name></expr>)",
/*4*/   R"(<expr><name>z</name></expr>)",
/*5*/   R"(<expr><literal type="number">1</literal> <operator>+</operator> <literal type="number">1</literal></expr>)",
/*6*/   R"(<expr><literal type="number">1</literal> <operator>+</operator> <literal type="number">2</literal></expr>)",
/*7*/   R"(<expr><literal type="number">2</literal> <operator>+</operator> <literal type="number">1</literal></expr>)",
/*8*/   R"(<expr><name>x</name> <operator>+</operator> <name>x</name></expr>)",
/*9*/   R"(<expr><name>x</name> <operator>+</operator> <name>y</name></expr>)",
/*10*/  R"(<expr><name>x</name> <operator>+</operator> <name>z</name></expr>)",
/*11*/  R"(<expr><name>y</name> <operator>+</operator> <name>x</name></expr>)",
/*12*/  R"(<expr><name>y</name> <operator>+</operator> <name>y</name></expr>)",
/*13*/  R"(<expr><name>y</name> <operator>+</operator> <name>z</name></expr>)",
/*14*/  R"(<expr><name>z</name> <operator>+</operator> <name>x</name></expr>)",
/*15*/  R"(<expr><name>z</name> <operator>+</operator> <name>y</name></expr>)",
/*16*/  R"(<expr><name>z</name> <operator>+</operator> <name>z</name></expr>)",
/*17*/  R"(<expr><name>x</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>x</name></expr>)",
/*18*/  R"(<expr><name>x</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>y</name></expr>)",
/*19*/  R"(<expr><name>x</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>z</name></expr>)",
/*20*/  R"(<expr><name>x</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>x</name></expr>)",
/*21*/  R"(<expr><name>x</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>y</name></expr>)",
/*22*/  R"(<expr><name>x</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>z</name></expr>)",
/*23*/  R"(<expr><name>x</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>x</name></expr>)",
/*24*/  R"(<expr><name>x</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>y</name></expr>)",
/*25*/  R"(<expr><name>x</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>z</name></expr>)",
/*26*/  R"(<expr><name>y</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>x</name></expr>)",
/*27*/  R"(<expr><name>y</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>y</name></expr>)",
/*28*/  R"(<expr><name>y</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>z</name></expr>)",
/*29*/  R"(<expr><name>y</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>x</name></expr>)",
/*30*/  R"(<expr><name>y</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>y</name></expr>)",
/*31*/  R"(<expr><name>y</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>z</name></expr>)",
/*32*/  R"(<expr><name>y</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>x</name></expr>)",
/*33*/  R"(<expr><name>y</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>y</name></expr>)",
/*34*/  R"(<expr><name>y</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>z</name></expr>)",
/*35*/  R"(<expr><name>z</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>x</name></expr>)",
/*36*/  R"(<expr><name>z</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>y</name></expr>)",
/*37*/  R"(<expr><name>z</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>z</name></expr>)",
/*38*/  R"(<expr><name>z</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>x</name></expr>)",
/*39*/  R"(<expr><name>z</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>y</name></expr>)",
/*40*/  R"(<expr><name>z</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>z</name></expr>)",
/*41*/  R"(<expr><name>z</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>x</name></expr>)",
/*42*/  R"(<expr><name>z</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>y</name></expr>)",
/*43*/  R"(<expr><name>z</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>z</name></expr>)"
    };
    const std::vector<std::string> expr_stmts_srcml {
/*0*/   R"(<expr_stmt><expr><literal type="number">1</literal></expr>;</expr_stmt>)",
/*1*/   R"(<expr_stmt><expr><literal type="number">2</literal></expr>;</expr_stmt>)",
/*2*/   R"(<expr_stmt><expr><name>x</name></expr>;</expr_stmt>)",
/*3*/   R"(<expr_stmt><expr><name>y</name></expr>;</expr_stmt>)",
/*4*/   R"(<expr_stmt><expr><name>z</name></expr>;</expr_stmt>)",
/*5*/   R"(<expr_stmt><expr><literal type="number">1</literal> <operator>+</operator> <literal type="number">1</literal></expr>;</expr_stmt>)",
/*6*/   R"(<expr_stmt><expr><literal type="number">1</literal> <operator>+</operator> <literal type="number">2</literal></expr>;</expr_stmt>)",
/*7*/   R"(<expr_stmt><expr><literal type="number">2</literal> <operator>+</operator> <literal type="number">1</literal></expr>;</expr_stmt>)",
/*8*/   R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*9*/   R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*10*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*11*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*12*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*13*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*14*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*15*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*16*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*17*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*18*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*19*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*20*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*21*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*22*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*23*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*24*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*25*/  R"(<expr_stmt><expr><name>x</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*26*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*27*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*28*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*29*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*30*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*31*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*32*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*33*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*34*/  R"(<expr_stmt><expr><name>y</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*35*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*36*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*37*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>x</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*38*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*39*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*40*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>y</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)",
/*41*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>x</name></expr>;</expr_stmt>)",
/*42*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>y</name></expr>;</expr_stmt>)",
/*43*/  R"(<expr_stmt><expr><name>z</name> <operator>+</operator> <name>z</name> <operator>+</operator> <name>z</name></expr>;</expr_stmt>)"
    };

    // $E
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 44);
        for(int i = 0; i < 42; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), exprs_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 44);
        for(int i = 0; i < 42; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), expr_stmts_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E1 + $E2;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E1 + $E2;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 39);
        for(int i = 5; i < 44; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i-5)), expr_stmts_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E1 + $E2 + $E3;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E1 + $E2 + $E3;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 27);
        for(int i = 17; i < 42; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i-17)), expr_stmts_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E + $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E + $E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 25);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), expr_stmts_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), expr_stmts_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), expr_stmts_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), expr_stmts_srcml[16]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), expr_stmts_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), expr_stmts_srcml[18]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), expr_stmts_srcml[19]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), expr_stmts_srcml[20]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), expr_stmts_srcml[21]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), expr_stmts_srcml[23]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), expr_stmts_srcml[25]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), expr_stmts_srcml[26]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), expr_stmts_srcml[27]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), expr_stmts_srcml[29]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), expr_stmts_srcml[30]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), expr_stmts_srcml[31]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), expr_stmts_srcml[33]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), expr_stmts_srcml[34]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), expr_stmts_srcml[35]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), expr_stmts_srcml[37]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), expr_stmts_srcml[39]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,21)), expr_stmts_srcml[40]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,22)), expr_stmts_srcml[41]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,23)), expr_stmts_srcml[42]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,24)), expr_stmts_srcml[43]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E + $E + $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E + $E + $E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), expr_stmts_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), expr_stmts_srcml[30]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), expr_stmts_srcml[43]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E1 + $E1 + $E2;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E1 + $E1 + $E2;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), expr_stmts_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), expr_stmts_srcml[18]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), expr_stmts_srcml[19]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), expr_stmts_srcml[29]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), expr_stmts_srcml[30]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), expr_stmts_srcml[31]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), expr_stmts_srcml[41]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), expr_stmts_srcml[42]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), expr_stmts_srcml[43]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E1 + $E2 + $E1;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E1 + $E2 + $E1;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), expr_stmts_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), expr_stmts_srcml[20]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), expr_stmts_srcml[23]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), expr_stmts_srcml[27]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), expr_stmts_srcml[30]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), expr_stmts_srcml[33]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), expr_stmts_srcml[37]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), expr_stmts_srcml[40]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), expr_stmts_srcml[43]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E1 + $E2 + $E2;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E1 + $E2 + $E2;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), expr_stmts_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), expr_stmts_srcml[21]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), expr_stmts_srcml[25]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), expr_stmts_srcml[26]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), expr_stmts_srcml[30]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), expr_stmts_srcml[34]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), expr_stmts_srcml[35]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), expr_stmts_srcml[39]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), expr_stmts_srcml[43]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    const std::string call_expressions = R"(
foo();
bar();
foo(foo());
foo(bar());
bar(foo());
bar(bar());
foo(1);
bar(1);
foo(1 + 2);
foo(1 + 1);
foo(bar() + bar());
foo() + foo();
foo() + bar();
bar() + foo();
bar() + bar();
foo(1) + foo(1);
foo(1) + foo(2);
foo(1) + bar(1);
)";


    const std::vector<std::string> call_exprs {
        R"(<expr><call><name>foo</name><argument_list>()</argument_list></call></expr>)",
        R"(<expr><call><name>bar</name><argument_list>()</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><call><name>foo</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><call><name>bar</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>bar</name><argument_list>(<argument><expr><call><name>foo</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>bar</name><argument_list>(<argument><expr><call><name>bar</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>bar</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal> <operator>+</operator> <literal type="number">2</literal></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal> <operator>+</operator> <literal type="number">1</literal></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><call><name>bar</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>bar</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>foo</name><argument_list>()</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>bar</name><argument_list>()</argument_list></call></expr>)",
        R"(<expr><call><name>bar</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>foo</name><argument_list>()</argument_list></call></expr>)",
        R"(<expr><call><name>bar</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>bar</name><argument_list>()</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call> <operator>+</operator> <call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call> <operator>+</operator> <call><name>foo</name><argument_list>(<argument><expr><literal type="number">2</literal></expr></argument>)</argument_list></call></expr>)",
        R"(<expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call> <operator>+</operator> <call><name>bar</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call></expr>)"
    };

    const std::vector<std::string> call_expr_stmts {
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>()</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>bar</name><argument_list>()</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><call><name>foo</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><call><name>bar</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>bar</name><argument_list>(<argument><expr><call><name>foo</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>bar</name><argument_list>(<argument><expr><call><name>bar</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>bar</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal> <operator>+</operator> <literal type="number">2</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal> <operator>+</operator> <literal type="number">1</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><call><name>bar</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>bar</name><argument_list>()</argument_list></call></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>foo</name><argument_list>()</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>bar</name><argument_list>()</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>bar</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>foo</name><argument_list>()</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>bar</name><argument_list>()</argument_list></call> <operator>+</operator> <call><name>bar</name><argument_list>()</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call> <operator>+</operator> <call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call> <operator>+</operator> <call><name>foo</name><argument_list>(<argument><expr><literal type="number">2</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call> <operator>+</operator> <call><name>bar</name><argument_list>(<argument><expr><literal type="number">1</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>)"
    };

    // $E
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 33);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[2]); // foo(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[3]); // foo(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), call_exprs[4]); // bar(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), call_exprs[5]); // bar(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), call_exprs[6]); // foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), exprs_srcml[0]); // 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), call_exprs[7]); // bar(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), exprs_srcml[0]); // 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), call_exprs[8]); // foo(1 + 2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), exprs_srcml[6]); // 1 + 2
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), call_exprs[9]); // foo(1 + 1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), exprs_srcml[5]); // 1 + 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), call_exprs[10]); // foo(bar() + bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), call_exprs[11]); // foo() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,21)), call_exprs[12]); // foo() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,22)), call_exprs[13]); // bar() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,23)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,24)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,25)), exprs_srcml[0]); // 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,26)), exprs_srcml[0]); // 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,27)), call_exprs[16]); // foo(1) + foo(2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,28)), exprs_srcml[0]); // 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,29)), exprs_srcml[1]); // 2
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,30)), call_exprs[17]); // foo(1) + bar(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,31)), exprs_srcml[0]); // 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,32)), exprs_srcml[0]); // 1

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 18);

        for(size_t i = 0; i < call_expr_stmts.size(); ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), call_expr_stmts[i]); 
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E1 + $E2
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E1 + $E2"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 10);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_srcml[6]); // 1 + 2
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_srcml[5]); // 1 + 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[11]); // foo() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[12]); // foo() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), call_exprs[13]); // bar() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), call_exprs[16]); // foo(1) + foo(2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), call_exprs[17]); // foo(1) + bar(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E + $E
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E + $E"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_srcml[5]); // 1 + 1
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[11]); // foo() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[15]); // foo(1) + foo(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $E + $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$E + $E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_expr_stmts[11]); // foo() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_expr_stmts[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_expr_stmts[15]); // foo(1) + foo(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL()
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL()"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 23);

        
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[2]); // foo(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[3]); // foo(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), call_exprs[4]); // bar(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), call_exprs[5]); // bar(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), call_exprs[6]); // foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), call_exprs[7]); // bar(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), call_exprs[8]); // foo(1 + 2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), call_exprs[9]); // foo(1 + 1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), call_exprs[10]); // foo(bar() + bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), call_exprs[11]); // foo() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), call_exprs[12]); // foo() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), call_exprs[13]); // bar() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,21)), call_exprs[16]); // foo(1) + foo(2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,22)), call_exprs[17]); // foo(1) + bar(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL();
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL();"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 18);

        for(size_t i = 0; i < call_expr_stmts.size(); ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), call_expr_stmts[i]);
        }


        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL($ARG)
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL($ARG)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 12);

        
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[2]); // foo(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[3]); // foo(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[4]); // bar(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[5]); // bar(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[6]); // foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), call_exprs[7]); // bar(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), call_exprs[8]); // foo(1 + 2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), call_exprs[9]); // foo(1 + 1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), call_exprs[10]); // foo(bar() + bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), call_exprs[16]); // foo(1) + foo(2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), call_exprs[17]); // foo(1) + bar(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL($ARG);
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL($ARG);"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 12);

        
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_expr_stmts[2]); // foo(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_expr_stmts[3]); // foo(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_expr_stmts[4]); // bar(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_expr_stmts[5]); // bar(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_expr_stmts[6]); // foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), call_expr_stmts[7]); // bar(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), call_expr_stmts[8]); // foo(1 + 2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), call_expr_stmts[9]); // foo(1 + 1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), call_expr_stmts[10]); // foo(bar() + bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), call_expr_stmts[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), call_expr_stmts[16]); // foo(1) + foo(2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), call_expr_stmts[17]); // foo(1) + bar(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL($ARG1 + $ARG2);
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL($ARG1 + $ARG2);"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_expr_stmts[8]); // foo(1 + 2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_expr_stmts[9]); // foo(1 + 1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_expr_stmts[10]); // foo(bar() + bar())

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL($ARG + $ARG);
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL($ARG + $ARG);"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_expr_stmts[9]); // foo(1 + 1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_expr_stmts[10]); // foo(bar() + bar())

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL1() + $CALL2()
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL1() + $CALL2()"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[11]); // foo() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[12]); // foo() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[13]); // bar() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), call_exprs[16]); // foo(1) + foo(2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), call_exprs[17]); // foo(1) + bar(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL() + $CALL()
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL() + $CALL()"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[11]); // foo() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[16]); // foo(1) + foo(2)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL($ARG1) + $CALL($ARG2)
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL($ARG1) + $CALL($ARG2)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[16]); // foo(1) + foo(2)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL($ARG) + $CALL($ARG)
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL($ARG) + $CALL($ARG)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[15]); // foo(1) + foo(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL1($ARG) + $CALL2($ARG)
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL1($ARG) + $CALL2($ARG)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[17]); // foo(1) + bar(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL1($CALL2())
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL1($CALL2())"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);

        
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[2]); // foo(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[3]); // foo(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[4]); // bar(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[5]); // bar(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[10]); // foo(bar() + bar())

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $CALL($CALL())
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$CALL($CALL())"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);

        
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[2]); // foo(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[5]); // bar(bar())

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    const std::string int_x_in_ifs = R"(
if (true) { }

if (true) { if (true) {} }

if (true) { 
    if (true) {}
}

if (true) { int x; }

if (true) { int y; }

if (true) { if (true) { int x; } }

if (true) { if (true) { int y; } }

if (true) {
    if (true) { int x; }
}

if (true) {
    if (true) { int y; }
}

if (true) { 
    int x;
}

if (true) { 
    int y;
}

if (true) {
if (true) { 
    int x;
}
}

if (true) {
if (true) { 
    int y;
}
}
)";

const std::string int_x_in_small_if_srcml = 
    R"(<if_stmt><if>if <condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt> </block_content>}</block></if></if_stmt>)";

const std::string int_x_in_big_if_srcml = 
    R"(<if_stmt><if>if <condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> 
    <decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt>
</block_content>}</block></if></if_stmt>)";

    // if () { }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_in_ifs.c_str(),int_x_in_ifs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"if () { }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 21);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // if (true) { int x; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_in_ifs.c_str(),int_x_in_ifs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"if (true) { int x; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_in_small_if_srcml)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_in_small_if_srcml)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_in_small_if_srcml)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), int_x_in_big_if_srcml)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), int_x_in_big_if_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // if () { if() {} }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_in_ifs.c_str(),int_x_in_ifs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"if () { if() {} }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string list_of_strings = R"(
"";
"x";
"FOO";
"aFOO";
"bFOO";
" FOO";
"FOOFOO";
"FOOa";
"FOOb";
)";

    // ""
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"("")"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">""</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // "x"
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"\"x\""), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">"x"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // "FOO"
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"\"FOO\""), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">"FOO"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // "$STR"
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"\"$STR\""), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">""</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr><literal type="string">"x"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr><literal type="string">"FOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr><literal type="string">"aFOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<expr><literal type="string">"bFOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string(R"(<expr><literal type="string">" FOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string(R"(<expr><literal type="string">"FOOFOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string(R"(<expr><literal type="string">"FOOa"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string(R"(<expr><literal type="string">"FOOb"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // "FOO$STR"
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"\"FOO$STR\""), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">"FOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr><literal type="string">"FOOFOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr><literal type="string">"FOOa"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr><literal type="string">"FOOb"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string decl_functions_src = R"(
void func1(int x) {
    char y = z;
}

int func2(char x) {
    int y = z;
}

int func3(char x) {
    char y = z;
}

int func4(int x) {
    char y = z;
}

int func5(int x) {
    int y = z;
}

int func6(char x) {
    int y = x;
}

int func7(char x) {
    char y = x;
}

int func8(int x) {
    char y = x;
}

int func9(int x) {
    int y = x;
}
)";

    const std::vector<std::string> decl_functions_srcml {
        R"(<function><type><name>void</name></type> <name>func1</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>char</name></type> <name>y</name> <init>= <expr><name>z</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>func2</name><parameter_list>(<parameter><decl><type><name>char</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>int</name></type> <name>y</name> <init>= <expr><name>z</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>func3</name><parameter_list>(<parameter><decl><type><name>char</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>char</name></type> <name>y</name> <init>= <expr><name>z</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>func4</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>char</name></type> <name>y</name> <init>= <expr><name>z</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>func5</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>int</name></type> <name>y</name> <init>= <expr><name>z</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>func6</name><parameter_list>(<parameter><decl><type><name>char</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>int</name></type> <name>y</name> <init>= <expr><name>x</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>func7</name><parameter_list>(<parameter><decl><type><name>char</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>char</name></type> <name>y</name> <init>= <expr><name>x</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>func8</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>char</name></type> <name>y</name> <init>= <expr><name>x</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>func9</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content>
    <decl_stmt><decl><type><name>int</name></type> <name>y</name> <init>= <expr><name>x</name></expr></init></decl>;</decl_stmt>
</block_content>}</block></function>)"
    };

    // $FUNCTYPE $FUNCNAME() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$FUNCTYPE $FUNCNAME() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        for(size_t i = 0; i < 9; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), decl_functions_srcml[i]);
        }
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $FUNCTYPE $FUNCNAME($PARAMTYPE $PARAM) {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$FUNCTYPE $FUNCNAME($PARAMTYPE $PARAM) {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        for(size_t i = 0; i < 9; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), decl_functions_srcml[i]);
        }
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $FUNCTYPE $FUNCNAME() { $DECLTYPE $VAR; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$FUNCTYPE $FUNCNAME() { $DECLTYPE $VAR; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        for(size_t i = 0; i < 9; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), decl_functions_srcml[i]);
        }
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $FUNCTYPE $FUNCNAME() { $DECLTYPE $VAR = $VAL; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$FUNCTYPE $FUNCNAME() { $DECLTYPE $VAR = $VAL; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        for(size_t i = 0; i < 9; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), decl_functions_srcml[i]);
        }
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $FUNCTYPE $FUNCNAME($PARAMTYPE $PARAM) { $DECLTYPE $VAR = $VAL; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$FUNCTYPE $FUNCNAME() { $DECLTYPE $VAR = $VAL; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        for(size_t i = 0; i < 9; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), decl_functions_srcml[i]);
        }
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $TYPE $FUNCNAME($PARAMTYPE $PARAM) { $TYPE $VAR = $VAL; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$TYPE $FUNCNAME($PARAMTYPE $PARAM) { $TYPE $VAR = $VAL; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), decl_functions_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), decl_functions_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $FUNCTYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $VAL; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$FUNCTYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $VAL; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), decl_functions_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), decl_functions_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $TYPE $FUNCNAME($TYPE $PARAM) { $DECLTYPE $VAR = $VAL; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$TYPE $FUNCNAME($TYPE $PARAM) { $DECLTYPE $VAR = $VAL; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), decl_functions_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), decl_functions_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $TYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $VAL; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$TYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $VAL; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $FUNCTYPE $FUNCNAME($PARAMTYPE $PARAM) { $DECLTYPE $VAR = $PARAM; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$FUNCTYPE $FUNCNAME($PARAMTYPE $PARAM) { $DECLTYPE $VAR = $PARAM; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), decl_functions_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), decl_functions_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $TYPE $FUNCNAME($PARAMTYPE $PARAM) { $TYPE $VAR = $PARAM; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$TYPE $FUNCNAME($PARAMTYPE $PARAM) { $TYPE $VAR = $PARAM; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $FUNCTYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $PARAM; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$FUNCTYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $PARAM; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $TYPE $FUNCNAME($TYPE $PARAM) { $DECLTYPE $VAR = $PARAM; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$TYPE $FUNCNAME($TYPE $PARAM) { $DECLTYPE $VAR = $PARAM; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // $TYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $PARAM; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"$TYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $PARAM; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    ////// Operations
    //// FINDs
    // FIND src:decl
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,"",0);
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND src:decl_stmt
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_stmt_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND cpp:include
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = include_iostream_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_iostream_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND //src:decl
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND //src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND //src:decl_stmt
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND //src:decl_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_stmt_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_stmt_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND //cpp:include
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = include_iostream_src + include_string_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND //cpp:include"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), include_iostream_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), include_string_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND //src:function/src:name
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_functions_src.c_str(),name_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND //src:function/src:name"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>foo</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>bar</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>baz</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND //src:function[src:type[src:name[.="void"]]]
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_functions_src.c_str(),name_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND //src:function[src:type[src:name[.="void"]]])"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<function><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND int $T
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src + int_x_function_decl_statement_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND int $N"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $T x
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src + int_x_function_decl_statement_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $T x"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $T $N
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src + int_x_function_decl_statement_src + int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $T $N"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $E
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $E"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 44);
        for(int i = 0; i < 42; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), exprs_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $E1 + $E2;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $E1 + $E2;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 39);
        for(int i = 5; i < 44; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i-5)), expr_stmts_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $E1 + $E2 + $E3;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $E1 + $E2 + $E3;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 27);
        for(int i = 17; i < 42; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i-17)), expr_stmts_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $E + $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $E + $E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 25);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), expr_stmts_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), expr_stmts_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), expr_stmts_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), expr_stmts_srcml[16]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), expr_stmts_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), expr_stmts_srcml[18]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), expr_stmts_srcml[19]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), expr_stmts_srcml[20]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), expr_stmts_srcml[21]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), expr_stmts_srcml[23]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), expr_stmts_srcml[25]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), expr_stmts_srcml[26]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), expr_stmts_srcml[27]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), expr_stmts_srcml[29]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), expr_stmts_srcml[30]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), expr_stmts_srcml[31]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), expr_stmts_srcml[33]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), expr_stmts_srcml[34]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), expr_stmts_srcml[35]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), expr_stmts_srcml[37]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), expr_stmts_srcml[39]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,21)), expr_stmts_srcml[40]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,22)), expr_stmts_srcml[41]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,23)), expr_stmts_srcml[42]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,24)), expr_stmts_srcml[43]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $E + $E + $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $E + $E + $E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), expr_stmts_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), expr_stmts_srcml[30]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), expr_stmts_srcml[43]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $CALL()
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $CALL()"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 23);

        
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), call_exprs[2]); // foo(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), call_exprs[3]); // foo(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), call_exprs[4]); // bar(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), call_exprs[0]); // foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), call_exprs[5]); // bar(bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), call_exprs[1]); // bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), call_exprs[6]); // foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), call_exprs[7]); // bar(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), call_exprs[8]); // foo(1 + 2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), call_exprs[9]); // foo(1 + 1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), call_exprs[10]); // foo(bar() + bar())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), call_exprs[11]); // foo() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), call_exprs[12]); // foo() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), call_exprs[13]); // bar() + foo()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), call_exprs[14]); // bar() + bar()
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), call_exprs[15]); // foo(1) + foo(1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,21)), call_exprs[16]); // foo(1) + foo(2)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,22)), call_exprs[17]); // foo(1) + bar(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $CALL($ARG + $ARG);
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $CALL($ARG + $ARG);"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_expr_stmts[9]); // foo(1 + 1)
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_expr_stmts[10]); // foo(bar() + bar())

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $CALL($ARG) + $CALL($ARG)
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $CALL($ARG) + $CALL($ARG)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[15]); // foo(1) + foo(1)

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $CALL($CALL())
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,call_expressions.c_str(),call_expressions.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $CALL($CALL())"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);

        
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), call_exprs[2]); // foo(foo())
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), call_exprs[5]); // bar(bar())

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if () { if() {} }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_in_ifs.c_str(),int_x_in_ifs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if () { if() {} }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND ""
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND "")"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">""</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND "FOO"
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND \"FOO\""), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">"FOO"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND "$STR"
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND \"$STR\""), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">""</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr><literal type="string">"x"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr><literal type="string">"FOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr><literal type="string">"aFOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<expr><literal type="string">"bFOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string(R"(<expr><literal type="string">" FOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string(R"(<expr><literal type="string">"FOOFOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string(R"(<expr><literal type="string">"FOOa"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string(R"(<expr><literal type="string">"FOOb"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND "FOO$STR"
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_strings.c_str(),list_of_strings.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND \"FOO$STR\""), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">"FOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr><literal type="string">"FOOFOO"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr><literal type="string">"FOOa"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr><literal type="string">"FOOb"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $FUNCTYPE $FUNCNAME() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $FUNCTYPE $FUNCNAME() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        for(size_t i = 0; i < 9; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), decl_functions_srcml[i]);
        }
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $PARAM; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $FUNCNAME($TYPE $PARAM) { $TYPE $VAR = $PARAM; }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), decl_functions_srcml[8]);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    //// Source Code Specifiers
    // FIND TAG src:decl
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        std::string input = int_x_decl_statement_src+int_x_function_decl_statement_src+int_x_parameter_src;
        srcml_unit_parse_memory(unit,input.c_str(),input.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND TAG src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), int_x_decl_srcml);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), int_x_decl_srcml);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND XPATH //src:function[src:type[src:name[.="void"]]]
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_functions_src.c_str(),name_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND XPATH //src:function[src:type[src:name[.="void"]]])"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<function><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND PATTERN $FUNCTYPE $FUNCNAME() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,decl_functions_src.c_str(),decl_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND PATTERN $FUNCTYPE $FUNCNAME() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        for(size_t i = 0; i < 9; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), decl_functions_srcml[i]);
        }
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string normal_label_src = R"(
jump_point:
    statement
)";

    const std::string normal_label_srcml = R"(<label><name>jump_point</name>:</label>)";

    const std::string src_label_src = R"(
src:
    label
)";

const std::string src_label_srcml = R"(<label><name>src</name>:</label>)";

    // FIND src:label
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        std::vector<std::string> unit_strings {normal_label_src,src_label_src};
        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,"C++");
            srcml_unit_parse_memory(unit,unit_strings[i].c_str(),unit_strings[i].size());
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);
            srcml_unit_free(unit);
        }

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:label"), SRCML_STATUS_OK);

        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            srcml_unit_apply_transforms(iarchive, unit, &result);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
            dassert(srcml_transform_get_unit_size(result), 1);
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), (i == 0 ? normal_label_srcml : src_label_srcml));

            srcml_unit_free(unit);
            srcml_transform_free(result);
        }

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND TAG src:label
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        std::vector<std::string> unit_strings {normal_label_src,src_label_src};
        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,"C++");
            srcml_unit_parse_memory(unit,unit_strings[i].c_str(),unit_strings[i].size());
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);
            srcml_unit_free(unit);
        }

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND TAG src:label"), SRCML_STATUS_OK);

        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            srcml_unit_apply_transforms(iarchive, unit, &result);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
            dassert(srcml_transform_get_unit_size(result), 1);
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), (i == 0 ? normal_label_srcml : src_label_srcml));

            srcml_unit_free(unit);
            srcml_transform_free(result);
        }

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND XPATH src:label
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        std::vector<std::string> unit_strings {normal_label_src,src_label_src};
        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,"C++");
            srcml_unit_parse_memory(unit,unit_strings[i].c_str(),unit_strings[i].size());
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);
            srcml_unit_free(unit);
        }

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND XPATH src:label"), SRCML_STATUS_OK);

        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            srcml_unit_apply_transforms(iarchive, unit, &result);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
            dassert(srcml_transform_get_unit_size(result), 1);
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), (i == 0 ? normal_label_srcml : src_label_srcml));

            srcml_unit_free(unit);
            srcml_transform_free(result);
        }

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND PATTERN src:label
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        std::vector<std::string> unit_strings {normal_label_src,src_label_src};
        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,"C++");
            srcml_unit_parse_memory(unit,unit_strings[i].c_str(),unit_strings[i].size());
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);
            srcml_unit_free(unit);
        }

        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND PATTERN src:label"), SRCML_STATUS_OK);

        for (size_t i = 0; i < unit_strings.size(); ++i) {
            srcml_unit* unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            srcml_unit_apply_transforms(iarchive, unit, &result);

            if(i == 0) {
                dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);
            }
            else {
                dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
                dassert(srcml_transform_get_unit_size(result), 1);
                dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), src_label_srcml);
            }

            srcml_unit_free(unit);
            srcml_transform_free(result);
        }

        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    //// CONTAINS
    const std::string contains_functions_src = R"(
void foo() { 
    if(true) { int x; } 
}

void bar() { 
    while(true) { 
        if(true) { int y; } 
    }
}

void baz(int z) { 
    while(true) { 
        ++z;
    }
}
)";

    std::vector<std::string> contains_functions_srcml {
        R"(<function><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content> 
    <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt> </block_content>}</block></if></if_stmt> 
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>bar</name><parameter_list>()</parameter_list> <block>{<block_content> 
    <while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> 
        <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <decl_stmt><decl><type><name>int</name></type> <name>y</name></decl>;</decl_stmt> </block_content>}</block></if></if_stmt> 
    </block_content>}</block></while>
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>baz</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>z</name></decl></parameter>)</parameter_list> <block>{<block_content> 
    <while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> 
        <expr_stmt><expr><operator>++</operator><name>z</name></expr>;</expr_stmt>
    </block_content>}</block></while>
</block_content>}</block></function>)"
    };

    // FIND void $FNAME() {} CONTAINS if() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,contains_functions_src.c_str(),contains_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void $FNAME() {} CONTAINS if() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), contains_functions_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), contains_functions_srcml[1]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND void $FNAME() { if() {} }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,contains_functions_src.c_str(),contains_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void $FNAME() { if() {} }"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), contains_functions_srcml[0]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string double_contains_functions_src = R"(
void foo() { 
    open("file.txt");
    close("file.txt");
}

void bar() { 
    close("file.txt");
}

void bar() { 
    open("file.txt");
}

void bar() { 
    close("file.txt");
    open("file.txt");
}
)";

    std::vector<std::string> double_contains_functions_srcml = {
        R"(<function><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content> 
    <expr_stmt><expr><call><name>open</name><argument_list>(<argument><expr><literal type="string">"file.txt"</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>
    <expr_stmt><expr><call><name>close</name><argument_list>(<argument><expr><literal type="string">"file.txt"</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>bar</name><parameter_list>()</parameter_list> <block>{<block_content> 
    <expr_stmt><expr><call><name>close</name><argument_list>(<argument><expr><literal type="string">"file.txt"</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>bar</name><parameter_list>()</parameter_list> <block>{<block_content> 
    <expr_stmt><expr><call><name>open</name><argument_list>(<argument><expr><literal type="string">"file.txt"</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>bar</name><parameter_list>()</parameter_list> <block>{<block_content> 
    <expr_stmt><expr><call><name>close</name><argument_list>(<argument><expr><literal type="string">"file.txt"</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>
    <expr_stmt><expr><call><name>open</name><argument_list>(<argument><expr><literal type="string">"file.txt"</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>
</block_content>}</block></function>)"
    };

    // FIND void $FNAME() {} CONTAINS open();
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,double_contains_functions_src.c_str(),double_contains_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void $FNAME() {} CONTAINS open();"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), double_contains_functions_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), double_contains_functions_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), double_contains_functions_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND void $FNAME() {} CONTAINS close();
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,double_contains_functions_src.c_str(),double_contains_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void $FNAME() {} CONTAINS close();"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), double_contains_functions_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), double_contains_functions_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), double_contains_functions_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND void $FNAME() {} CONTAINS open(); CONTAINS close();
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,double_contains_functions_src.c_str(),double_contains_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void $FNAME() {} CONTAINS open(); CONTAINS close();"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), double_contains_functions_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), double_contains_functions_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND void $FNAME() {} CONTAINS close(); CONTAINS open();
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,double_contains_functions_src.c_str(),double_contains_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void $FNAME() {} CONTAINS close(); CONTAINS open();"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), double_contains_functions_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), double_contains_functions_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string various_contains_exprs_src = R"(
void foo() {
    a;
}

if(true) {
    a;
}

while(true) {
    a;
}

for(;;) {
    a;
}

void foo() {
    b;
}

if(true) {
    b;
}

while(true) {
    b;
}

for(;;) {
    b;
}
)";

    const std::vector<std::string> various_contains_exprs_srcml {
        R"(<function><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content>
    <expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</block_content>}</block></function>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content>
    <expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</block_content>}</block></if></if_stmt>)",
        R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content>
    <expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</block_content>}</block></while>)",
        R"(<for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content>
    <expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</block_content>}</block></for>)",
        R"(<function><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content>
    <expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</block_content>}</block></function>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content>
    <expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</block_content>}</block></if></if_stmt>)",
        R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content>
    <expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</block_content>}</block></while>)",
        R"(<for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content>
    <expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</block_content>}</block></for>)"
};

    /* TODO - Tests "work", but returns more than expected and there appears to be an issue with get_srcml_inner
              This is likely because one of the results is a unit itself, so the unit peeling gets messed up
    */
    // // FIND XPATH * CONTAINS a;
    // {
    //     char* s;
    //     size_t size;

    //     srcml_archive* oarchive = srcml_archive_create();
    //     srcml_archive_write_open_memory(oarchive,&s, &size);

    //     srcml_unit* unit = srcml_unit_create(oarchive);
    //     srcml_unit_set_language(unit,"C++");
    //     srcml_unit_parse_memory(unit,various_contains_exprs_src.c_str(),various_contains_exprs_src.size());
    //     dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

    //     srcml_unit_free(unit);
    //     srcml_archive_close(oarchive);
    //     srcml_archive_free(oarchive);

    //     std::string srcml_text = std::string(s, size);
    //     free(s);

    //     srcml_archive* iarchive = srcml_archive_create();
    //     srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
    //     dassert(srcml_append_transform_srcql(iarchive,"FIND XPATH * CONTAINS a;"), SRCML_STATUS_OK);

    //     unit = srcml_archive_read_unit(iarchive);
    //     srcml_transform_result* result = nullptr;
    //     srcml_unit_apply_transforms(iarchive, unit, &result);

    //     dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
    //     dassert(srcml_transform_get_unit_size(result), 4);
    //     dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), various_contains_exprs_srcml[0]);
    //     dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), various_contains_exprs_srcml[1]);
    //     dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), various_contains_exprs_srcml[2]);
    //     dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), various_contains_exprs_srcml[3]);

    //     srcml_unit_free(unit);
    //     srcml_transform_free(result);
    //     srcml_archive_close(iarchive);
    //     srcml_archive_free(iarchive);
    // }

    // // FIND XPATH * CONTAINS b;
    // {
    //     char* s;
    //     size_t size;

    //     srcml_archive* oarchive = srcml_archive_create();
    //     srcml_archive_write_open_memory(oarchive,&s, &size);

    //     srcml_unit* unit = srcml_unit_create(oarchive);
    //     srcml_unit_set_language(unit,"C++");
    //     srcml_unit_parse_memory(unit,various_contains_exprs_src.c_str(),various_contains_exprs_src.size());
    //     dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

    //     srcml_unit_free(unit);
    //     srcml_archive_close(oarchive);
    //     srcml_archive_free(oarchive);

    //     std::string srcml_text = std::string(s, size);
    //     free(s);

    //     srcml_archive* iarchive = srcml_archive_create();
    //     srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
    //     dassert(srcml_append_transform_srcql(iarchive,"FIND XPATH * CONTAINS b;"), SRCML_STATUS_OK);

    //     unit = srcml_archive_read_unit(iarchive);
    //     srcml_transform_result* result = nullptr;
    //     srcml_unit_apply_transforms(iarchive, unit, &result);

    //     dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
    //     dassert(srcml_transform_get_unit_size(result), 4);
    //     dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), various_contains_exprs_srcml[4]);
    //     dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), various_contains_exprs_srcml[5]);
    //     dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), various_contains_exprs_srcml[6]);
    //     dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), various_contains_exprs_srcml[7]);

    //     srcml_unit_free(unit);
    //     srcml_transform_free(result);
    //     srcml_archive_close(iarchive);
    //     srcml_archive_free(iarchive);
    // }

    // // FIND XPATH * CONTAINS $T;
    // {
    //     char* s;
    //     size_t size;

    //     srcml_archive* oarchive = srcml_archive_create();
    //     srcml_archive_write_open_memory(oarchive,&s, &size);

    //     srcml_unit* unit = srcml_unit_create(oarchive);
    //     srcml_unit_set_language(unit,"C++");
    //     srcml_unit_parse_memory(unit,various_contains_exprs_src.c_str(),various_contains_exprs_src.size());
    //     dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

    //     srcml_unit_free(unit);
    //     srcml_archive_close(oarchive);
    //     srcml_archive_free(oarchive);

    //     std::string srcml_text = std::string(s, size);
    //     free(s);

    //     srcml_archive* iarchive = srcml_archive_create();
    //     srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
    //     dassert(srcml_append_transform_srcql(iarchive,"FIND XPATH * CONTAINS b;"), SRCML_STATUS_OK);

    //     unit = srcml_archive_read_unit(iarchive);
    //     srcml_transform_result* result = nullptr;
    //     srcml_unit_apply_transforms(iarchive, unit, &result);

    //     dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
    //     dassert(srcml_transform_get_unit_size(result), 4);
    //     for(size_t i = 0; i < 8; ++i) {
    //         dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), various_contains_exprs_srcml[i]);
    //     }

    //     srcml_unit_free(unit);
    //     srcml_transform_free(result);
    //     srcml_archive_close(iarchive);
    //     srcml_archive_free(iarchive);
    // }

    //// FOLLOWED BY
    const std::string followed_functions_src = R"(
void foo1() {
    open();
    close();
}

void foo2() {
    close();
    open();
}

void foo3() {
    open();
    if(condition) {
        close();
    }
}

void foo4() {
    if(open()) {
        close();
    }
}

void foo5() {
    if(condition) {
        open();
    }
    close();
}
)";

    const std::vector<std::string> followed_functions_srcml {
        R"(<function><type><name>void</name></type> <name>foo1</name><parameter_list>()</parameter_list> <block>{<block_content>
    <expr_stmt><expr><call><name>open</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
    <expr_stmt><expr><call><name>close</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>foo2</name><parameter_list>()</parameter_list> <block>{<block_content>
    <expr_stmt><expr><call><name>close</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
    <expr_stmt><expr><call><name>open</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>foo3</name><parameter_list>()</parameter_list> <block>{<block_content>
    <expr_stmt><expr><call><name>open</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
    <if_stmt><if>if<condition>(<expr><name>condition</name></expr>)</condition> <block>{<block_content>
        <expr_stmt><expr><call><name>close</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
    </block_content>}</block></if></if_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>foo4</name><parameter_list>()</parameter_list> <block>{<block_content>
    <if_stmt><if>if<condition>(<expr><call><name>open</name><argument_list>()</argument_list></call></expr>)</condition> <block>{<block_content>
        <expr_stmt><expr><call><name>close</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
    </block_content>}</block></if></if_stmt>
</block_content>}</block></function>)",
        R"(<function><type><name>void</name></type> <name>foo5</name><parameter_list>()</parameter_list> <block>{<block_content>
    <if_stmt><if>if<condition>(<expr><name>condition</name></expr>)</condition> <block>{<block_content>
        <expr_stmt><expr><call><name>open</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
    </block_content>}</block></if></if_stmt>
    <expr_stmt><expr><call><name>close</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
</block_content>}</block></function>)"
    };

    // FIND void $FNAME() {} CONTAINS open() FOLLOWED BY close()
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,followed_functions_src.c_str(),followed_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void $FNAME() {} CONTAINS open() FOLLOWED BY close()"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_functions_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), followed_functions_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), followed_functions_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), followed_functions_srcml[4]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND void $FNAME() {} CONTAINS close() FOLLOWED BY open()
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,followed_functions_src.c_str(),followed_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void $FNAME() {} CONTAINS close() FOLLOWED BY open()"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_functions_srcml[1]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    const std::string followed_ifs_src = R"(
if(true) { a; b; c; }

if(true) { a; c; b; }

if(true) { b; a; c; }

if(true) { b; c; a; }

if(true) { c; a; b; }

if(true) { c; b; a; }

if(true) { if(true) { a; } b; c; }

if(true) { a; if(true) { b; } c; }

if(true) { a; b; if(true) { c; } }

if(true) { if(true) { a; } if(true) { b; } c; }

if(true) { if(true) { a; } b; if(true) { c; } }

if(true) { a; if(true) { b; } if(true) { c; } }

if(true) { if(true) { a; } if(true) { b; } if(true) { c; } }

if(true) { if(true) { a; } if(true) { if(true) { b; } } if(true) { c; } }
)";

    const std::vector<std::string> followed_ifs_srcml {
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></if></if_stmt>)",
        R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>b</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></if></if_stmt> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></if></if_stmt>)"
    };

    // FIND if(true) {} CONTAINS $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,(followed_ifs_src+"//COMMENT1").c_str(),(followed_ifs_src+"//COMMENT1").size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS $E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 30);
        
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS a;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,(followed_ifs_src+"//COMMENT2").c_str(),(followed_ifs_src+"//COMMENT2").size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS a;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 19);
        
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS b;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,(followed_ifs_src+"//COMMENT3").c_str(),(followed_ifs_src+"//COMMENT3").size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS b;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 20);
        
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS c;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,(followed_ifs_src+"//COMMENT4").c_str(),(followed_ifs_src+"//COMMENT4").size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS c;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 19);
        
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS a; FOLLOWED BY b;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,(followed_ifs_src+"//COMMENT5").c_str(),(followed_ifs_src+"//COMMENT5").size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS a; FOLLOWED BY b;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 11);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_ifs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), followed_ifs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), followed_ifs_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), followed_ifs_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), followed_ifs_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), followed_ifs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), followed_ifs_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), followed_ifs_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), followed_ifs_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), followed_ifs_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), followed_ifs_srcml[13]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS a; FOLLOWED BY c;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,(followed_ifs_src+"//COMMENT6").c_str(),(followed_ifs_src+"//COMMENT6").size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS a; FOLLOWED BY c;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 11);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_ifs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), followed_ifs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), followed_ifs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), followed_ifs_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), followed_ifs_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), followed_ifs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), followed_ifs_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), followed_ifs_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), followed_ifs_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), followed_ifs_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), followed_ifs_srcml[13]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS b; FOLLOWED BY c;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,(followed_ifs_src+"//COMMENT7").c_str(),(followed_ifs_src+"//COMMENT7").size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS b; FOLLOWED BY c;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 11);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_ifs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), followed_ifs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), followed_ifs_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), followed_ifs_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), followed_ifs_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), followed_ifs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), followed_ifs_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), followed_ifs_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), followed_ifs_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), followed_ifs_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), followed_ifs_srcml[13]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS a; FOLLOWED BY b; FOLLOWED BY c;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,(followed_ifs_src+"//COMMENT8").c_str(),(followed_ifs_src+"//COMMENT8").size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS a; FOLLOWED BY b; FOLLOWED BY c;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_ifs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), followed_ifs_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), followed_ifs_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), followed_ifs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), followed_ifs_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), followed_ifs_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), followed_ifs_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), followed_ifs_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), followed_ifs_srcml[13]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS if(){} FOLLOWED BY if(){} FOLLOWED BY if(){}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,followed_ifs_src.c_str(),followed_ifs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:if_stmt CONTAINS src:if_stmt FOLLOWED BY src:if_stmt FOLLOWED BY src:if_stmt"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_ifs_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), followed_ifs_srcml[13]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if(true) {} CONTAINS a; FOLLOWED BY b; CONTAINS b; FOLLOWED BY c;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,followed_ifs_src.c_str(),followed_ifs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if(true) {} CONTAINS a; FOLLOWED BY b; CONTAINS b; FOLLOWED BY c;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_ifs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), followed_ifs_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), followed_ifs_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), followed_ifs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), followed_ifs_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), followed_ifs_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), followed_ifs_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), followed_ifs_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), followed_ifs_srcml[13]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    const std::string functions_in_classes_src = R"(
void foo1() {}

void foo2() { a; }

void foo3(int b) {}

class EMPTY {};

class A {
    void foo4() {}
};

class B {
    void foo5() {}
    void foo6() {}
};

class C {
    void foo7() { c; }
    void foo8(int d) {}
};

struct A {
    void foo9() {}
    void foo10() { e; }
};
)";

const std::vector<std::string> functions_in_classes_srcml {
    "<function><type><name>void</name></type> <name>foo1</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>",
    "<function><type><name>void</name></type> <name>foo2</name><parameter_list>()</parameter_list> <block>{<block_content> <expr_stmt><expr><name>a</name></expr>;</expr_stmt> </block_content>}</block></function>",
    "<function><type><name>void</name></type> <name>foo3</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>b</name></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></function>",
    "<function><type><name>void</name></type> <name>foo4</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>",
    "<function><type><name>void</name></type> <name>foo5</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>",
    "<function><type><name>void</name></type> <name>foo6</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>",
    "<function><type><name>void</name></type> <name>foo7</name><parameter_list>()</parameter_list> <block>{<block_content> <expr_stmt><expr><name>c</name></expr>;</expr_stmt> </block_content>}</block></function>",
    "<function><type><name>void</name></type> <name>foo8</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>d</name></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></function>",
    "<function><type><name>void</name></type> <name>foo9</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>",
    "<function><type><name>void</name></type> <name>foo10</name><parameter_list>()</parameter_list> <block>{<block_content> <expr_stmt><expr><name>e</name></expr>;</expr_stmt> </block_content>}</block></function>"
};

    //// WITHIN
    // FIND src:function WITHIN src:class
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:function WITHIN src:class"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), functions_in_classes_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), functions_in_classes_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), functions_in_classes_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), functions_in_classes_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), functions_in_classes_srcml[7]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND src:function WITHIN src:struct
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:function WITHIN src:struct"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), functions_in_classes_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), functions_in_classes_srcml[9]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND src:function WITHIN XPATH *
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:function WITHIN XPATH *"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 10);
        for (size_t i = 0; i < 10; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), functions_in_classes_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN src:function
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN src:function"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>a</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>c</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN src:class
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN src:class"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>c</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN src:struct
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN src:struct"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN src:block
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN src:block"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>a</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>c</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>a</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>c</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN XPATH *
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN XPATH *"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>a</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>c</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND int $X WITHIN src:function
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND int $X WITHIN src:function"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<decl><type><name>int</name></type> <name>b</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<decl><type><name>int</name></type> <name>d</name></decl>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND int $X WITHIN src:class
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND int $X WITHIN src:class"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<decl><type><name>int</name></type> <name>d</name></decl>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND int $X WITHIN src:struct
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND int $X WITHIN src:struct"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);
        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND int $X WITHIN src:class
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,functions_in_classes_src.c_str(),functions_in_classes_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND int $X WITHIN src:class"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<decl><type><name>int</name></type> <name>d</name></decl>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string various_within_exprs_src = R"(
if(true) { a; }

while(true) { b; }

for(;;) { c; }

if(true) { while(true) { d; } }

while(true) { if(true) { e; } }

for(;;) { if(true) { f; } }

for(;;) { while(true) { g; } }

for(;;) { if(true) { while(true) { h; } } }

for(;;) { while(true) { if(true) { i; } } }

if(true) { for(;;) { while(true) { j; } } }

if(true) { while(true) { for(;;) { k; } } }

while(true) { for(;;) { if(true) { l; } } }

while(true) { if(true) { for(;;) { m; } } }
)";

    // FIND $X; WITHIN if() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN if() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 10);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>a</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>d</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<expr_stmt><expr><name>f</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<expr_stmt><expr><name>h</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<expr_stmt><expr><name>i</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<expr_stmt><expr><name>j</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<expr_stmt><expr><name>k</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string("<expr_stmt><expr><name>l</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), std::string("<expr_stmt><expr><name>m</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN while() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN while() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 10);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>b</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>d</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<expr_stmt><expr><name>g</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<expr_stmt><expr><name>h</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<expr_stmt><expr><name>i</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<expr_stmt><expr><name>j</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<expr_stmt><expr><name>k</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string("<expr_stmt><expr><name>l</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), std::string("<expr_stmt><expr><name>m</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN if() {} WITHIN while() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN if() {} WITHIN while() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>d</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>h</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<expr_stmt><expr><name>i</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<expr_stmt><expr><name>j</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<expr_stmt><expr><name>k</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<expr_stmt><expr><name>l</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<expr_stmt><expr><name>m</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN while() {} WITHIN if() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN while() {} WITHIN if() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>d</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>h</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<expr_stmt><expr><name>i</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<expr_stmt><expr><name>j</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<expr_stmt><expr><name>k</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<expr_stmt><expr><name>l</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<expr_stmt><expr><name>m</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; WITHIN if() {} WITHIN while() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; WITHIN if() {} WITHIN while() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<expr_stmt><expr><name>d</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<expr_stmt><expr><name>e</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<expr_stmt><expr><name>h</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<expr_stmt><expr><name>i</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<expr_stmt><expr><name>j</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<expr_stmt><expr><name>k</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<expr_stmt><expr><name>l</name></expr>;</expr_stmt>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<expr_stmt><expr><name>m</name></expr>;</expr_stmt>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if() {} WITHIN while() {} CONTAINS $X;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if() {} WITHIN while() {} CONTAINS $X;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>e</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>i</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>l</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <expr_stmt><expr><name>m</name></expr>;</expr_stmt> </block_content>}</block></for> </block_content>}</block></if></if_stmt>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND while() {} WITHIN if() {} CONTAINS $X;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND while() {} WITHIN if() {} CONTAINS $X;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>d</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>h</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>j</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <expr_stmt><expr><name>k</name></expr>;</expr_stmt> </block_content>}</block></for> </block_content>}</block></while>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND while() {} WITHIN if() {} CONTAINS $X;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND while() {} WITHIN if() {} CONTAINS $X;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>d</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>h</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>j</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <expr_stmt><expr><name>k</name></expr>;</expr_stmt> </block_content>}</block></for> </block_content>}</block></while>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND if() {} CONTAINS $X; WITHIN while() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND if() {} CONTAINS $X; WITHIN while() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>d</name></expr>;</expr_stmt> </block_content>}</block></while> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>e</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>h</name></expr>;</expr_stmt> </block_content>}</block></while> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>i</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>j</name></expr>;</expr_stmt> </block_content>}</block></while> </block_content>}</block></for> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <expr_stmt><expr><name>k</name></expr>;</expr_stmt> </block_content>}</block></for> </block_content>}</block></while> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>l</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string(R"(<if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <expr_stmt><expr><name>m</name></expr>;</expr_stmt> </block_content>}</block></for> </block_content>}</block></if></if_stmt>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND while() {} CONTAINS $X; WITHIN if() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_within_exprs_src.c_str(),various_within_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND while() {} CONTAINS $X; WITHIN if() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>d</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>e</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>h</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>i</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>j</name></expr>;</expr_stmt> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <expr_stmt><expr><name>k</name></expr>;</expr_stmt> </block_content>}</block></for> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <expr_stmt><expr><name>l</name></expr>;</expr_stmt> </block_content>}</block></if></if_stmt> </block_content>}</block></for> </block_content>}</block></while>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string(R"(<while>while<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <if_stmt><if>if<condition>(<expr><literal type="boolean">true</literal></expr>)</condition> <block>{<block_content> <for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content> <expr_stmt><expr><name>m</name></expr>;</expr_stmt> </block_content>}</block></for> </block_content>}</block></if></if_stmt> </block_content>}</block></while>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    //// FROM
    const std::string named_funcs_src = R"(
void foo() {}

int foo1() {}

bool foo2() {}

float bar(int x) {}

char* baz(int y) {}

class X {
    X bin() {}
};

class Y {};
)";

    // FIND src:name FROM FIND src:function
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:name FROM FIND src:function"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 16);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>void</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>foo</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<name>foo1</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<name>bool</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<name>foo2</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<name>float</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<name>bar</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), std::string("<name>x</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), std::string("<name>char</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), std::string("<name>baz</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), std::string("<name>y</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), std::string("<name>X</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), std::string("<name>bin</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND src:name FROM FIND $TYPE $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:name FROM FIND $TYPE $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 16);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>void</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>foo</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<name>foo1</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<name>bool</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<name>foo2</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<name>float</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<name>bar</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), std::string("<name>x</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), std::string("<name>char</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), std::string("<name>baz</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), std::string("<name>y</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), std::string("<name>X</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), std::string("<name>bin</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND //src:name FROM FIND $TYPE $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND //src:name FROM FIND $TYPE $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 16);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>void</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>foo</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<name>foo1</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<name>bool</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<name>foo2</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<name>float</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<name>bar</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), std::string("<name>x</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), std::string("<name>char</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), std::string("<name>baz</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), std::string("<name>y</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), std::string("<name>X</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), std::string("<name>bin</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND /src:name FROM FIND $TYPE $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND /src:name FROM FIND $TYPE $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 6);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>foo</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>foo1</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>foo2</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<name>bar</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<name>baz</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<name>bin</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND /src:name FROM FIND $TYPE foo$FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND /src:name FROM FIND $TYPE foo$FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>foo</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>foo1</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>foo2</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND src:type FROM FIND $TYPE $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND src:type FROM FIND $TYPE $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<type><name>void</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<type><name>int</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<type><name>bool</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<type><name>float</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<type><name>int</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<type><name>char</name><modifier>*</modifier></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<type><name>int</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<type><name>X</name></type>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND /src:type FROM FIND $TYPE $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND /src:type FROM FIND $TYPE $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 6);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<type><name>void</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<type><name>int</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<type><name>bool</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<type><name>float</name></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<type><name>char</name><modifier>*</modifier></type>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<type><name>X</name></type>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND /src:name FROM FIND /src:type FROM FIND $TYPE $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND /src:name FROM FIND /src:type FROM FIND $TYPE $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 6);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>void</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>int</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>bool</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<name>float</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<name>char</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<name>X</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND /src:name FROM class $C {};
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"/src:name FROM class $C {};"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>X</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>Y</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND /src:name FROM class $C {}; CONTAINS $C $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,named_funcs_src.c_str(),named_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND /src:name FROM class $C {}; CONTAINS $C $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>X</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    //// UNION
    // FIND src:expr UNION FIND src:decl
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,int_x_in_ifs.c_str(),int_x_in_ifs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"src:expr UNION FIND src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 31);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<decl><type><name>int</name></type> <name>x</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string("<decl><type><name>int</name></type> <name>y</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), std::string("<decl><type><name>int</name></type> <name>x</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), std::string("<decl><type><name>int</name></type> <name>y</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), std::string("<decl><type><name>int</name></type> <name>x</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), std::string("<decl><type><name>int</name></type> <name>y</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,21)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,22)), std::string("<decl><type><name>int</name></type> <name>x</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,23)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,24)), std::string("<decl><type><name>int</name></type> <name>y</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,25)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,26)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,27)), std::string("<decl><type><name>int</name></type> <name>x</name></decl>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,28)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,29)), std::string(R"(<expr><literal type="boolean">true</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,30)), std::string("<decl><type><name>int</name></type> <name>y</name></decl>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; UNION FIND $Y;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_contains_exprs_src.c_str(),various_contains_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; UNION FIND $Y;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr_stmt><expr><name>a</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr_stmt><expr><name>a</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr_stmt><expr><name>a</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr_stmt><expr><name>a</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<expr_stmt><expr><name>b</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string(R"(<expr_stmt><expr><name>b</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string(R"(<expr_stmt><expr><name>b</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string(R"(<expr_stmt><expr><name>b</name></expr>;</expr_stmt>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $X; UNION FIND $X;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,various_contains_exprs_src.c_str(),various_contains_exprs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $X; UNION FIND $X;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr_stmt><expr><name>a</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr_stmt><expr><name>a</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr_stmt><expr><name>a</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr_stmt><expr><name>a</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<expr_stmt><expr><name>b</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string(R"(<expr_stmt><expr><name>b</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string(R"(<expr_stmt><expr><name>b</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string(R"(<expr_stmt><expr><name>b</name></expr>;</expr_stmt>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string parameter_same_type_funcs_src = R"(
int one(int x) {}

bool two(bool y) {}

char* three(char* z) {}

void four(void* a) {}

float five(double b) {}
)";

    const std::vector<std::string> parameter_same_type_funcs_srcml {
        "<function><type><name>int</name></type> <name>one</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></function>",
        "<function><type><name>bool</name></type> <name>two</name><parameter_list>(<parameter><decl><type><name>bool</name></type> <name>y</name></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></function>",
        "<function><type><name>char</name><modifier>*</modifier></type> <name>three</name><parameter_list>(<parameter><decl><type><name>char</name><modifier>*</modifier></type> <name>z</name></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></function>",
        "<function><type><name>void</name></type> <name>four</name><parameter_list>(<parameter><decl><type><name>void</name><modifier>*</modifier></type> <name>a</name></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></function>",
        "<function><type><name>float</name></type> <name>five</name><parameter_list>(<parameter><decl><type><name>double</name></type> <name>b</name></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></function>"
    };

    const std::vector<std::string> parameter_same_type_funcs_params_srcml {
        "<decl><type><name>int</name></type> <name>x</name></decl>",
        "<decl><type><name>bool</name></type> <name>y</name></decl>",
        "<decl><type><name>char</name><modifier>*</modifier></type> <name>z</name></decl>",
        "<decl><type><name>void</name><modifier>*</modifier></type> <name>a</name></decl>",
        "<decl><type><name>double</name></type> <name>b</name></decl>",
    };

    // FIND $T1 $FUNC($T1 $PARAM) {} UNION FIND $T2 $PARAM WITHIN $T2 $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,parameter_same_type_funcs_src.c_str(),parameter_same_type_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $T1 $FUNC($T1 $PARAM) {} UNION FIND $T2 $PARAM WITHIN $T2 $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), parameter_same_type_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), parameter_same_type_funcs_params_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), parameter_same_type_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), parameter_same_type_funcs_params_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), parameter_same_type_funcs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), parameter_same_type_funcs_params_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), parameter_same_type_funcs_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), parameter_same_type_funcs_params_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $FUNC($TYPE $PARAM) {} UNION FIND $TYPE $PARAM WITHIN $TYPE $FUNC() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,parameter_same_type_funcs_src.c_str(),parameter_same_type_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $FUNC($TYPE $PARAM) {} UNION FIND $TYPE $PARAM WITHIN $TYPE $FUNC() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 8);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), parameter_same_type_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), parameter_same_type_funcs_params_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), parameter_same_type_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), parameter_same_type_funcs_params_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), parameter_same_type_funcs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), parameter_same_type_funcs_params_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), parameter_same_type_funcs_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), parameter_same_type_funcs_params_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $T x UNION FIND $T y UNION FIND $T z UNION FIND $T a UNION FIND $T b
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,parameter_same_type_funcs_src.c_str(),parameter_same_type_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $T x UNION FIND $T y UNION FIND $T z UNION FIND $T a UNION FIND $T b"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);

        for(size_t i = 0; i < 5; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), parameter_same_type_funcs_params_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND /src:name FROM FIND src:function UNION FIND /src:name FROM FIND src:decl
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,parameter_same_type_funcs_src.c_str(),parameter_same_type_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND /src:name FROM FIND src:function UNION FIND /src:name FROM FIND src:decl"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 10);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string("<name>one</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string("<name>x</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string("<name>two</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string("<name>y</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string("<name>three</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string("<name>z</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string("<name>four</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string("<name>a</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string("<name>five</name>"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), std::string("<name>b</name>"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    //// INTERSECT
    // FIND x INTERSECT FIND y
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND x INTERSECT FIND y"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        // for(int i = 0; i < srcml_transform_get_unit_size(result); ++i) {
        //     std::cout << "!!!!" << i << ": " << srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)) << std::endl;
        // }
        dassert(srcml_transform_get_unit_size(result), 14);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_srcml[18]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), exprs_srcml[20]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), exprs_srcml[21]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), exprs_srcml[22]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), exprs_srcml[24]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), exprs_srcml[26]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), exprs_srcml[27]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), exprs_srcml[28]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), exprs_srcml[29]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), exprs_srcml[32]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), exprs_srcml[36]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), exprs_srcml[38]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND x INTERSECT FIND y INTERSECT FIND z
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND x INTERSECT FIND y INTERSECT FIND z"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 6);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_srcml[22]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_srcml[24]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_srcml[28]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), exprs_srcml[32]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), exprs_srcml[36]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), exprs_srcml[38]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND x INTERSECT FIND x
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND x INTERSECT FIND x"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 25);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), exprs_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), exprs_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), exprs_srcml[14]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), exprs_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), exprs_srcml[18]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), exprs_srcml[19]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), exprs_srcml[20]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), exprs_srcml[21]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), exprs_srcml[22]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), exprs_srcml[23]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), exprs_srcml[24]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), exprs_srcml[25]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), exprs_srcml[26]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), exprs_srcml[27]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), exprs_srcml[28]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), exprs_srcml[29]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), exprs_srcml[32]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), exprs_srcml[35]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,21)), exprs_srcml[36]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,22)), exprs_srcml[37]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,23)), exprs_srcml[38]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,24)), exprs_srcml[41]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND 0 INTERSECT FIND 0
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND 0 INTERSECT FIND 0"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND x INTERSECT FIND 1
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND x INTERSECT FIND 1"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }




    //// DIFFERENCE
    // FIND x DIFFERENCE FIND y
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND x DIFFERENCE FIND y"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 11);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), exprs_srcml[14]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), exprs_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), exprs_srcml[19]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), exprs_srcml[23]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), exprs_srcml[25]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), exprs_srcml[35]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), exprs_srcml[37]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), exprs_srcml[41]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND x DIFFERENCE FIND y DIFFERENCE FIND z
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND x DIFFERENCE FIND y DIFFERENCE FIND z"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);

        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_srcml[17]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    // FIND $E + $E; DIFFERENCE FIND $A + $B + $C; UNION FIND $E + $E + $E;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,list_of_exprs.c_str(),list_of_exprs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $E + $E; DIFFERENCE FIND $A + $B + $C; UNION FIND $E + $E + $E;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 7);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), expr_stmts_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), expr_stmts_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), expr_stmts_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), expr_stmts_srcml[16]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), expr_stmts_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), expr_stmts_srcml[30]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), expr_stmts_srcml[43]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    ////// WHERE
    const std::string name_style_funcs_src = R"(
void foo() {}
int snake_case(int a) { a + 1; }
bool long_snake_case(int b, bool c) { b + c; }
char CAP_SNAKE_CASE(int c, int d) { c + 1; d + 1;}
int camelCase(int e, int f, int g) { _e + 1; _f + 1; _g + 1 }
int PascalCase() { call(); }
)";

    const std::vector<std::string> name_style_funcs_srcml {
        R"(<function><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>snake_case</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>a</name></decl></parameter>)</parameter_list> <block>{<block_content> <expr_stmt><expr><name>a</name> <operator>+</operator> <literal type="number">1</literal></expr>;</expr_stmt> </block_content>}</block></function>)",
        R"(<function><type><name>bool</name></type> <name>long_snake_case</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>b</name></decl></parameter>, <parameter><decl><type><name>bool</name></type> <name>c</name></decl></parameter>)</parameter_list> <block>{<block_content> <expr_stmt><expr><name>b</name> <operator>+</operator> <name>c</name></expr>;</expr_stmt> </block_content>}</block></function>)",
        R"(<function><type><name>char</name></type> <name>CAP_SNAKE_CASE</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>c</name></decl></parameter>, <parameter><decl><type><name>int</name></type> <name>d</name></decl></parameter>)</parameter_list> <block>{<block_content> <expr_stmt><expr><name>c</name> <operator>+</operator> <literal type="number">1</literal></expr>;</expr_stmt> <expr_stmt><expr><name>d</name> <operator>+</operator> <literal type="number">1</literal></expr>;</expr_stmt></block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>camelCase</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>e</name></decl></parameter>, <parameter><decl><type><name>int</name></type> <name>f</name></decl></parameter>, <parameter><decl><type><name>int</name></type> <name>g</name></decl></parameter>)</parameter_list> <block>{<block_content> <expr_stmt><expr><name>_e</name> <operator>+</operator> <literal type="number">1</literal></expr>;</expr_stmt> <expr_stmt><expr><name>_f</name> <operator>+</operator> <literal type="number">1</literal></expr>;</expr_stmt> <expr_stmt><expr><name>_g</name> <operator>+</operator> <literal type="number">1</literal></expr></expr_stmt> </block_content>}</block></function>)",
        R"(<function><type><name>int</name></type> <name>PascalCase</name><parameter_list>()</parameter_list> <block>{<block_content> <expr_stmt><expr><call><name>call</name><argument_list>()</argument_list></call></expr>;</expr_stmt> </block_content>}</block></function>)"
    };


    //// MATCH
    // FIND $TYPE $NAME() {} WHERE MATCH($NAME,"[a-z]+(?:_[a-z]+)*")
    {
        std::cout << "-----------------------------------------------------" << std::endl;
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE MATCH($NAME,\"[a-z]+(?:_[a-z]+)*\")"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        std::cout << "EX: " << srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)) << std::endl;
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), name_style_funcs_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME($TYPE $PARAM) {} WHERE MATCH($TYPE,"int|bool")
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME($TYPE $PARAM) {} WHERE MATCH($TYPE,\"int|bool\")"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), name_style_funcs_srcml[4]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME($TYPE $PARAM) {} WHERE MATCH($NAME,"[a-z]+(?:_[a-z]+)*") WHERE MATCH($TYPE,"int|bool")
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME($TYPE $PARAM) {} WHERE MATCH($NAME,\"[a-z]+(?:_[a-z]+)*\") WHERE MATCH($TYPE,\"int|bool\")"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    //// NOT
    // FIND $TYPE $NAME() {} WHERE NOT(int $NAME() {})
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE NOT(int $NAME() {})"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), name_style_funcs_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} CONTAINS src:expr_stmt WHERE NOT($E + 1;)
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} CONTAINS src:expr_stmt WHERE NOT($E + 1;)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE NOT(int $NAME() {}) CONTAINS src:expr_stmt WHERE NOT($E + 1;)
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE NOT(int $NAME() {}) CONTAINS src:expr_stmt WHERE NOT($E + 1;)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    //// COUNT
    // FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 0
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 0"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 1
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 1"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[1]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 2
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 2"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 3
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 3"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[4]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 4
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) = 4"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) < 1
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) < 1"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) <= 1
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT(src:parameter) <= 1"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), name_style_funcs_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT($E + 1;) > 2
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT($E + 1;) > 2"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[4]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT($E + 1;) < 2
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT($E + 1;) < 2"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), name_style_funcs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), name_style_funcs_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} WHERE COUNT($E) < 3
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} WHERE COUNT($E) < 3"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), name_style_funcs_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $TYPE $NAME() {} CONTAINS src:block WHERE COUNT($E) < 3
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,name_style_funcs_src.c_str(),name_style_funcs_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $TYPE $NAME() {} CONTAINS src:block WHERE COUNT($E) < 3"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), name_style_funcs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), name_style_funcs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), name_style_funcs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), name_style_funcs_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), name_style_funcs_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    ////// OTHER
    //// Prefix Unification
    // FIND void foo() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,followed_functions_src.c_str(),followed_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void foo() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND void foo1() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,followed_functions_src.c_str(),followed_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void foo1() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), followed_functions_srcml[0]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND void foo$T() {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,followed_functions_src.c_str(),followed_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND void foo$T() {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);
        for(size_t i = 0; i < 5; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), followed_functions_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    const std::string string_functions_src = R"(
char* x() { return "x"; }
char* y() { return "y"; }
char* z() { return "z"; }
char* a() { return "b"; }
)";

    const std::vector<std::string> string_functions_srcml {
        R"(<function><type><name>char</name><modifier>*</modifier></type> <name>x</name><parameter_list>()</parameter_list> <block>{<block_content> <return>return <expr><literal type="string">"x"</literal></expr>;</return> </block_content>}</block></function>)",
        R"(<function><type><name>char</name><modifier>*</modifier></type> <name>y</name><parameter_list>()</parameter_list> <block>{<block_content> <return>return <expr><literal type="string">"y"</literal></expr>;</return> </block_content>}</block></function>)",
        R"(<function><type><name>char</name><modifier>*</modifier></type> <name>z</name><parameter_list>()</parameter_list> <block>{<block_content> <return>return <expr><literal type="string">"z"</literal></expr>;</return> </block_content>}</block></function>)",
        R"(<function><type><name>char</name><modifier>*</modifier></type> <name>a</name><parameter_list>()</parameter_list> <block>{<block_content> <return>return <expr><literal type="string">"b"</literal></expr>;</return> </block_content>}</block></function>)"
    };

    //// String Unification
    // FIND char* $X() { return "$STR"; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,string_functions_src.c_str(),string_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND char* $X() { return "$STR"; })"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), string_functions_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), string_functions_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), string_functions_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), string_functions_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    // FIND char* $STR() { return "$STR"; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,string_functions_src.c_str(),string_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND char* $STR() { return "$STR"; })"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), string_functions_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), string_functions_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), string_functions_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND "$X" FROM FIND char* $X() { return "$STR"; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,string_functions_src.c_str(),string_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND "$STR" FROM FIND char* $X() { return "$STR"; })"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">"x"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr><literal type="string">"y"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr><literal type="string">"z"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr><literal type="string">"b"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND "$STR" FROM FIND char* $STR() { return "$STR"; }
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,string_functions_src.c_str(),string_functions_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND "$STR" FROM FIND char* $STR() { return "$STR"; })"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<expr><literal type="string">"x"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr><literal type="string">"y"</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<expr><literal type="string">"z"</literal></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }



    const std::string couts_src = R"(
cout << "x: " << x;
cout << "y: " << y;
cout << "z: " << z;
cout << "a: " << b;
cout << "1: " << 1;
cout << "foo(): " << foo();
cout << "bar(a): " << bar(a);
)";

    const std::vector<std::string> couts_srcml {
        R"(<expr_stmt><expr><name>cout</name> <operator>&lt;&lt;</operator> <literal type="string">"x: "</literal> <operator>&lt;&lt;</operator> <name>x</name></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><name>cout</name> <operator>&lt;&lt;</operator> <literal type="string">"y: "</literal> <operator>&lt;&lt;</operator> <name>y</name></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><name>cout</name> <operator>&lt;&lt;</operator> <literal type="string">"z: "</literal> <operator>&lt;&lt;</operator> <name>z</name></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><name>cout</name> <operator>&lt;&lt;</operator> <literal type="string">"a: "</literal> <operator>&lt;&lt;</operator> <name>b</name></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><name>cout</name> <operator>&lt;&lt;</operator> <literal type="string">"1: "</literal> <operator>&lt;&lt;</operator> <literal type="number">1</literal></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><name>cout</name> <operator>&lt;&lt;</operator> <literal type="string">"foo(): "</literal> <operator>&lt;&lt;</operator> <call><name>foo</name><argument_list>()</argument_list></call></expr>;</expr_stmt>)",
        R"(<expr_stmt><expr><name>cout</name> <operator>&lt;&lt;</operator> <literal type="string">"bar(a): "</literal> <operator>&lt;&lt;</operator> <call><name>bar</name><argument_list>(<argument><expr><name>a</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>)"
    };

    // FIND cout << "$STR: " << $X;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,couts_src.c_str(),couts_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND cout << "$STR: " << $X;)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 7);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), couts_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), couts_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), couts_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), couts_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), couts_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), couts_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), couts_srcml[6]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND cout << "$STR: " << $STR;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,couts_src.c_str(),couts_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND cout << "$STR: " << $STR;)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), couts_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), couts_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), couts_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), couts_srcml[4]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND src:literal FROM FIND cout << "$STR: " << $STR;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,couts_src.c_str(),couts_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,R"(FIND src:literal FROM FIND cout << "$STR: " << $STR;)"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<literal type="string">"x: "</literal>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<literal type="string">"y: "</literal>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<literal type="string">"z: "</literal>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<literal type="string">"1: "</literal>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<literal type="number">1</literal>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string exprs_and_decls_src = R"(
int x;
x;
int y = a;
int z = x;
int foo(int i) {}
for(int j = 0; j < 10; ++j) {}
)";

    //// expr and decl combination
    // FIND $T
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_src.c_str(),exprs_and_decls_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $T"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 11);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<decl><type><name>int</name></type> <name>x</name></decl>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr><name>x</name></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<decl><type><name>int</name></type> <name>y</name> <init>= <expr><name>a</name></expr></init></decl>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<expr><name>a</name></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), std::string(R"(<decl><type><name>int</name></type> <name>z</name> <init>= <expr><name>x</name></expr></init></decl>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), std::string(R"(<expr><name>x</name></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), std::string(R"(<decl><type><name>int</name></type> <name>i</name></decl>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), std::string(R"(<decl><type><name>int</name></type> <name>j</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), std::string(R"(<expr><literal type="number">0</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), std::string(R"(<expr><name>j</name> <operator>&lt;</operator> <literal type="number">10</literal></expr>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), std::string(R"(<expr><operator>++</operator><name>j</name></expr>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND $T;
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_src.c_str(),exprs_and_decls_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND $T;"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), std::string(R"(<decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), std::string(R"(<expr_stmt><expr><name>x</name></expr>;</expr_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), std::string(R"(<decl_stmt><decl><type><name>int</name></type> <name>y</name> <init>= <expr><name>a</name></expr></init></decl>;</decl_stmt>)"));
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), std::string(R"(<decl_stmt><decl><type><name>int</name></type> <name>z</name> <init>= <expr><name>x</name></expr></init></decl>;</decl_stmt>)"));

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    const std::string exprs_and_decls_in_fors_src = R"(
for(int a = 0; a < 10; ++a) {}
for(a = 10; a != 0; --a) {}
for(int b = 10; b != 10; --b) {}
for(b = 0; b < 10; ++b) {}
for(int c = 0; b != 10; ++a) {}
for(auto i : list) {}
for(;;) {}
)";

    const std::vector<std::string> exprs_and_decls_in_fors_srcml {
        R"(<for>for<control>(<init><decl><type><name>int</name></type> <name>a</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</init> <condition><expr><name>a</name> <operator>&lt;</operator> <literal type="number">10</literal></expr>;</condition> <incr><expr><operator>++</operator><name>a</name></expr></incr>)</control> <block>{<block_content/>}</block></for>)",
        R"(<for>for<control>(<init><expr><name>a</name> <operator>=</operator> <literal type="number">10</literal></expr>;</init> <condition><expr><name>a</name> <operator>!=</operator> <literal type="number">0</literal></expr>;</condition> <incr><expr><operator>--</operator><name>a</name></expr></incr>)</control> <block>{<block_content/>}</block></for>)",
        R"(<for>for<control>(<init><decl><type><name>int</name></type> <name>b</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>;</init> <condition><expr><name>b</name> <operator>!=</operator> <literal type="number">10</literal></expr>;</condition> <incr><expr><operator>--</operator><name>b</name></expr></incr>)</control> <block>{<block_content/>}</block></for>)",
        R"(<for>for<control>(<init><expr><name>b</name> <operator>=</operator> <literal type="number">0</literal></expr>;</init> <condition><expr><name>b</name> <operator>&lt;</operator> <literal type="number">10</literal></expr>;</condition> <incr><expr><operator>++</operator><name>b</name></expr></incr>)</control> <block>{<block_content/>}</block></for>)",
        R"(<for>for<control>(<init><decl><type><name>int</name></type> <name>c</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</init> <condition><expr><name>b</name> <operator>!=</operator> <literal type="number">10</literal></expr>;</condition> <incr><expr><operator>++</operator><name>a</name></expr></incr>)</control> <block>{<block_content/>}</block></for>)",
        R"(<for>for<control>(<init><decl><type><name>auto</name></type> <name>i</name> <range>: <expr><name>list</name></expr></range></decl></init>)</control> <block>{<block_content/>}</block></for>)",
        R"(<for>for<control>(<init>;</init><condition>;</condition><incr/>)</control> <block>{<block_content/>}</block></for>)"
    };

    // FIND for
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_in_fors_src.c_str(),exprs_and_decls_in_fors_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND for"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 7);
        for(size_t i = 0; i < 7; ++i) {
            dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,i)), exprs_and_decls_in_fors_srcml[i]);
        }

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND for($T) {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_in_fors_src.c_str(),exprs_and_decls_in_fors_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND for($T) {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 6);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_and_decls_in_fors_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_and_decls_in_fors_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_and_decls_in_fors_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), exprs_and_decls_in_fors_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), exprs_and_decls_in_fors_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), exprs_and_decls_in_fors_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND for($T;;) {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_in_fors_src.c_str(),exprs_and_decls_in_fors_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"FIND for($T;;) {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 5);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_and_decls_in_fors_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_and_decls_in_fors_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_and_decls_in_fors_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), exprs_and_decls_in_fors_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), exprs_and_decls_in_fors_srcml[4]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND for($T;;$T) {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_in_fors_src.c_str(),exprs_and_decls_in_fors_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"for($T;;$T) {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_and_decls_in_fors_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_and_decls_in_fors_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_and_decls_in_fors_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), exprs_and_decls_in_fors_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND for($T;$T;$T) {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_in_fors_src.c_str(),exprs_and_decls_in_fors_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"for($T;$T;$T) {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 4);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_and_decls_in_fors_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_and_decls_in_fors_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), exprs_and_decls_in_fors_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), exprs_and_decls_in_fors_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND for($T;;++$T) {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_in_fors_src.c_str(),exprs_and_decls_in_fors_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"for($T;;++$T) {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_and_decls_in_fors_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_and_decls_in_fors_srcml[3]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // FIND for($T;;--$T) {}
    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,exprs_and_decls_in_fors_src.c_str(),exprs_and_decls_in_fors_src.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_srcql(iarchive,"for($T;;--$T) {}"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), exprs_and_decls_in_fors_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), exprs_and_decls_in_fors_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }
}

