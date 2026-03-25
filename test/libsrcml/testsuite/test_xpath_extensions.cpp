// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_xpath_extensions.cpp
 *
 * @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for special srcML extension functions
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
        dassert(srcml_append_transform_xpath(iarchive,"//src:unit//*"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_NONE);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    //// is-static
    // C++
    const std::string static_constructs_cpp = R"(
static int a;
static int b = 5;
static int c, d;
static const int e;
const static int f;
static void foo() {}
class Foo {
    static void bar() {}
};
)";

    const std::vector<std::string> static_constructs_cpp_srcml = {
        R"(<decl_stmt><decl><type><specifier>static</specifier> <name>int</name></type> <name>a</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>static</specifier> <name>int</name></type> <name>a</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>static</specifier> <name>int</name></type> <name>b</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>static</specifier> <name>int</name></type> <name>b</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>static</specifier> <name>int</name></type> <name>c</name></decl>, <decl><type ref="prev"/><name>d</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>static</specifier> <name>int</name></type> <name>c</name></decl>)",
        R"(<decl><type ref="prev"/><name>d</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>static</specifier> <specifier>const</specifier> <name>int</name></type> <name>e</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>static</specifier> <specifier>const</specifier> <name>int</name></type> <name>e</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <specifier>static</specifier> <name>int</name></type> <name>f</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <specifier>static</specifier> <name>int</name></type> <name>f</name></decl>)",
        R"(<function><type><specifier>static</specifier> <name>void</name></type> <name>foo</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)",
        R"(<class>class <name>Foo</name> <block>{<private type="default">
    <function><type><specifier>static</specifier> <name>void</name></type> <name>bar</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>
</private>}</block>;</class>)"
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,static_constructs_cpp.c_str(),static_constructs_cpp.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-static(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 13);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), static_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), static_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), static_constructs_cpp_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), static_constructs_cpp_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), static_constructs_cpp_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), static_constructs_cpp_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), static_constructs_cpp_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), static_constructs_cpp_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), static_constructs_cpp_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), static_constructs_cpp_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), static_constructs_cpp_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), static_constructs_cpp_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), static_constructs_cpp_srcml[12]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Java
    const std::string static_constructs_java = R"(
class A {
    static int count = 0;
}

class A {
    static {
        System.out.println("Class loaded");
    }
}

class Outer {
    static class Nested {}
}

class A {
    static final int MAX = 100;
}

interface A {
    static void helper() {}
}
)";

    const std::vector<std::string> static_constructs_java_srcml = {
        R"(<decl_stmt><decl><type><specifier>static</specifier> <name>int</name></type> <name>count</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>static</specifier> <name>int</name></type> <name>count</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>)",
        R"(<static>static <block>{<block_content>
        <expr_stmt><expr><call><name><name>System</name><operator>.</operator><name>out</name><operator>.</operator><name>println</name></name><argument_list>(<argument><expr><literal type="string">"Class loaded"</literal></expr></argument>)</argument_list></call></expr>;</expr_stmt>
    </block_content>}</block></static>)",
        R"(<class><specifier>static</specifier> class <name>Nested</name> <block>{}</block></class>)",
        R"(<decl_stmt><decl><type><specifier>static</specifier> <specifier>final</specifier> <name>int</name></type> <name>MAX</name> <init>= <expr><literal type="number">100</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>static</specifier> <specifier>final</specifier> <name>int</name></type> <name>MAX</name> <init>= <expr><literal type="number">100</literal></expr></init></decl>)",
        R"(<function><type><specifier>static</specifier> <name>void</name></type> <name>helper</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)"
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"Java");
        srcml_unit_parse_memory(unit,static_constructs_java.c_str(),static_constructs_java.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-static(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 7);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), static_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), static_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), static_constructs_cpp_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), static_constructs_cpp_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), static_constructs_cpp_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), static_constructs_cpp_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), static_constructs_cpp_srcml[6]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    // C#
    const std::string static_constructs_cs = R"(
class A {
    public static int Count = 0;
}

class A {
    public static void Foo() {}
}

class A {
    static A() {}
}

static class Utils {
    public static void Helper() {}
}

class A {
    public static int Value { get; set; }
}

void Outer() {
    static void Inner() {}
}

interface INumber<T> {
    static abstract T Zero { get; }
}
)";

    const std::vector<std::string> static_constructs_cs_srcml = {
        R"(<decl_stmt><decl><type><specifier>public</specifier> <specifier>static</specifier> <name>int</name></type> <name>Count</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>public</specifier> <specifier>static</specifier> <name>int</name></type> <name>Count</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>)",
        R"(function><type><specifier>public</specifier> <specifier>static</specifier> <name>void</name></type> <name>Foo</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)",
        R"(<constructor><specifier>static</specifier> <name>A</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></constructor>)",
        R"(<class><specifier>static</specifier> class <name>Utils</name> <block>{
    <function><type><specifier>public</specifier> <specifier>static</specifier> <name>void</name></type> <name>Helper</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>
}</block></class>)",
        R"(<function><type><specifier>public</specifier> <specifier>static</specifier> <name>void</name></type> <name>Helper</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)",
        R"(<property><type><specifier>public</specifier> <specifier>static</specifier> <name>int</name></type> <name>Value</name> <block>{ <function_decl><name>get</name>;</function_decl> <function_decl><name>set</name>;</function_decl> }</block></property>)",
        R"(<function><type><specifier>static</specifier> <name>void</name></type> <name>Inner</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)",
        R"(<property><type><specifier>static</specifier> <specifier>abstract</specifier> <name>T</name></type> <name>Zero</name> <block>{ <function_decl><name>get</name>;</function_decl> }</block></property>)"
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C#");
        srcml_unit_parse_memory(unit,static_constructs_cs.c_str(),static_constructs_cs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-static(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 9);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), static_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), static_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), static_constructs_cpp_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), static_constructs_cpp_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), static_constructs_cpp_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), static_constructs_cpp_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), static_constructs_cpp_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), static_constructs_cpp_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), static_constructs_cpp_srcml[8]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Python
    const std::string static_constructs_py = R"(
class A:
    count = 0

class A:
    @staticmethod
    def foo():
        pass


class A:
    @classmethod
    def foo(cls):
        pass
)";

    const std::vector<std::string> static_constructs_py_srcml = {
        R"(<expr_stmt><expr><name>count</name> <operator>=</operator> <literal type="number">0</literal></expr></expr_stmt>)",
        R"(<function><attribute>@<expr><name>staticmethod</name></expr></attribute>
    def <name>foo</name><parameter_list>()</parameter_list><block>:<block_content>
        <pass>pass</pass>
</block_content></block></function>)",
        R"(<function><attribute>@<expr><name>classmethod</name></expr></attribute>
    def <name>foo</name><parameter_list>(<parameter><name>cls</name></parameter>)</parameter_list><block>:<block_content>
        <pass>pass</pass>
</block_content></block></function>)"
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"Python");
        srcml_unit_parse_memory(unit,static_constructs_py.c_str(),static_constructs_py.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-static(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), static_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), static_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), static_constructs_cpp_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }



}
