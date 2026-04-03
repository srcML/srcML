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

    //// is-static()
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
        R"(<function><type><specifier>static</specifier> <name>void</name></type> <name>bar</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)"
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
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), static_constructs_java_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), static_constructs_java_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), static_constructs_java_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), static_constructs_java_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), static_constructs_java_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), static_constructs_java_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), static_constructs_java_srcml[6]);

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
        R"(<function><type><specifier>public</specifier> <specifier>static</specifier> <name>void</name></type> <name>Foo</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></function>)",
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
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), static_constructs_cs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), static_constructs_cs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), static_constructs_cs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), static_constructs_cs_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), static_constructs_cs_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), static_constructs_cs_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), static_constructs_cs_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), static_constructs_cs_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), static_constructs_cs_srcml[8]);

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
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), static_constructs_py_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), static_constructs_py_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), static_constructs_py_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Function Exists check
    {
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::cout << srcml_get_language_list(i) << std::endl;
            char* s;
            size_t size;

            srcml_archive* oarchive = srcml_archive_create();
            srcml_archive_write_open_memory(oarchive,&s, &size);

            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,srcml_get_language_list(i));
            srcml_unit_parse_memory(unit,"",0);
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

            srcml_unit_free(unit);
            srcml_archive_close(oarchive);
            srcml_archive_free(oarchive);

            std::string srcml_text = std::string(s, size);
            free(s);

            srcml_archive* iarchive = srcml_archive_create();
            srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
            dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-static(.) or true()]"), SRCML_STATUS_OK);

            unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            dassert(srcml_unit_apply_transforms(iarchive, unit, &result), SRCML_STATUS_OK);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        }
    }


    //// is-inline()
    // C++
    const std::string inline_constructs_cpp = R"(
inline int sum(int a, int b) { return a + b; }

inline std::atomic<int> counter(0);
)";

    const std::vector<std::string> inline_constructs_cpp_srcml = {
        R"(<function><type><specifier>inline</specifier> <name>int</name></type> <name>sum</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>a</name></decl></parameter>, <parameter><decl><type><name>int</name></type> <name>b</name></decl></parameter>)</parameter_list> <block>{<block_content> <return>return <expr><name>a</name> <operator>+</operator> <name>b</name></expr>;</return> </block_content>}</block></function>)",
        R"(<decl_stmt><decl><type><specifier>inline</specifier> <name><name>std</name><operator>::</operator><name>atomic</name><argument_list type="generic">&lt;<argument><expr><name>int</name></expr></argument>&gt;</argument_list></name></type> <name>counter</name><argument_list>(<argument><expr><literal type="number">0</literal></expr></argument>)</argument_list></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>inline</specifier> <name><name>std</name><operator>::</operator><name>atomic</name><argument_list type="generic">&lt;<argument><expr><name>int</name></expr></argument>&gt;</argument_list></name></type> <name>counter</name><argument_list>(<argument><expr><literal type="number">0</literal></expr></argument>)</argument_list></decl>)"
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,inline_constructs_cpp.c_str(),inline_constructs_cpp.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-inline(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), inline_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), inline_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), inline_constructs_cpp_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Function Exists check
    {
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::cout << srcml_get_language_list(i) << std::endl;
            char* s;
            size_t size;

            srcml_archive* oarchive = srcml_archive_create();
            srcml_archive_write_open_memory(oarchive,&s, &size);

            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,srcml_get_language_list(i));
            srcml_unit_parse_memory(unit,"",0);
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

            srcml_unit_free(unit);
            srcml_archive_close(oarchive);
            srcml_archive_free(oarchive);

            std::string srcml_text = std::string(s, size);
            free(s);

            srcml_archive* iarchive = srcml_archive_create();
            srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
            dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-inline(.) or true()]"), SRCML_STATUS_OK);

            unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            dassert(srcml_unit_apply_transforms(iarchive, unit, &result), SRCML_STATUS_OK);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        }
    }



    //// is-pure-virtual()
    // C++
    const std::string pure_virtual_constructs_cpp = R"(
class Base {
    int foo() = 0;
    virtual ~Base() = 0;
};
)";

    const std::vector<std::string> pure_virtual_constructs_cpp_srcml = {
        R"(<function_decl><type><name>int</name></type> <name>foo</name><parameter_list>()</parameter_list> = <literal type="number">0</literal>;</function_decl>)",
        R"(<destructor_decl><specifier>virtual</specifier> <name>~Base</name><parameter_list>()</parameter_list> = <literal type="number">0</literal>;</destructor_decl>)",
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,pure_virtual_constructs_cpp.c_str(),pure_virtual_constructs_cpp.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-pure-virtual(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), pure_virtual_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), pure_virtual_constructs_cpp_srcml[1]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    // C#
    const std::string pure_virtual_constructs_cs = R"(
abstract class Base {
    public abstract void DoWork();
}

interface IWorker {
    void DoWork();
}
)";

    const std::vector<std::string> pure_virtual_constructs_cs_srcml = {
        R"(<function_decl><type><specifier>public</specifier> <specifier>abstract</specifier> <name>void</name></type> <name>DoWork</name><parameter_list>()</parameter_list>;</function_decl>)",
        R"(<function_decl><type><name>void</name></type> <name>DoWork</name><parameter_list>()</parameter_list>;</function_decl>)",
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C#");
        srcml_unit_parse_memory(unit,pure_virtual_constructs_cs.c_str(),pure_virtual_constructs_cs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-pure-virtual(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), pure_virtual_constructs_cs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), pure_virtual_constructs_cs_srcml[1]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Java
    const std::string pure_virtual_constructs_java = R"(
abstract class Base {
    abstract void doWork();
}

interface Worker {
    void doWork();
}
)";

    const std::vector<std::string> pure_virtual_constructs_java_srcml = {
        R"(<function_decl><type><specifier>abstract</specifier> <name>void</name></type> <name>doWork</name><parameter_list>()</parameter_list>;</function_decl>)",
        R"(<function_decl><type><name>void</name></type> <name>doWork</name><parameter_list>()</parameter_list>;</function_decl>)",
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"Java");
        srcml_unit_parse_memory(unit,pure_virtual_constructs_java.c_str(),pure_virtual_constructs_java.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-pure-virtual(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 2);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), pure_virtual_constructs_java_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), pure_virtual_constructs_java_srcml[1]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Function Exists check
    {
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::cout << srcml_get_language_list(i) << std::endl;
            char* s;
            size_t size;

            srcml_archive* oarchive = srcml_archive_create();
            srcml_archive_write_open_memory(oarchive,&s, &size);

            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,srcml_get_language_list(i));
            srcml_unit_parse_memory(unit,"",0);
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

            srcml_unit_free(unit);
            srcml_archive_close(oarchive);
            srcml_archive_free(oarchive);

            std::string srcml_text = std::string(s, size);
            free(s);

            srcml_archive* iarchive = srcml_archive_create();
            srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
            dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-pure-virtual(.) or true()]"), SRCML_STATUS_OK);

            unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            dassert(srcml_unit_apply_transforms(iarchive, unit, &result), SRCML_STATUS_OK);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        }
    }


    //// is-explicit
    // C++
    const std::string explicit_constructs_cpp = R"(
struct B
{
    explicit B(int) {}
    explicit B(int, int) {}
    explicit operator bool() const { return true; }
};
)";

    const std::vector<std::string> explicit_constructs_cpp_srcml = {
        R"(<constructor><specifier>explicit</specifier> <name>B</name><parameter_list>(<parameter><decl><type><name>int</name></type></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></constructor>)",
        R"(<constructor><specifier>explicit</specifier> <name>B</name><parameter_list>(<parameter><decl><type><name>int</name></type></decl></parameter>, <parameter><decl><type><name>int</name></type></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></constructor>)",
        R"(<function type="operator"><type><specifier>explicit</specifier></type> <name>operator <name>bool</name></name><parameter_list>()</parameter_list> <specifier>const</specifier> <block>{<block_content> <return>return <expr><literal type="boolean">true</literal></expr>;</return> </block_content>}</block></function>)"
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,explicit_constructs_cpp.c_str(),explicit_constructs_cpp.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-explicit(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 3);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), explicit_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), explicit_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), explicit_constructs_cpp_srcml[2]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // C#
    const std::string explicit_constructs_cs = R"(
public class Product
{
    public static explicit operator string(Product p) {}
}
)";

    const std::vector<std::string> explicit_constructs_cs_srcml = {
        R"(<function type="operator"><type><specifier>public</specifier> <specifier>static</specifier> <specifier>explicit</specifier></type> <name>operator <name>string</name></name><parameter_list>(<parameter><decl><type><name>Product</name></type> <name>p</name></decl></parameter>)</parameter_list> <block>{<block_content/>}</block></function>)",
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C#");
        srcml_unit_parse_memory(unit,explicit_constructs_cs.c_str(),explicit_constructs_cs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-explicit(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 1);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), explicit_constructs_cs_srcml[0]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Function Exists check
    {
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::cout << srcml_get_language_list(i) << std::endl;
            char* s;
            size_t size;

            srcml_archive* oarchive = srcml_archive_create();
            srcml_archive_write_open_memory(oarchive,&s, &size);

            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,srcml_get_language_list(i));
            srcml_unit_parse_memory(unit,"",0);
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

            srcml_unit_free(unit);
            srcml_archive_close(oarchive);
            srcml_archive_free(oarchive);

            std::string srcml_text = std::string(s, size);
            free(s);

            srcml_archive* iarchive = srcml_archive_create();
            srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
            dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-explicit(.) or true()]"), SRCML_STATUS_OK);

            unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            dassert(srcml_unit_apply_transforms(iarchive, unit, &result), SRCML_STATUS_OK);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        }
    }



    //// is-deleted
    // C++
    const std::string deleted_constructs_cpp = R"(
void printInt(int x) {}
void printInt(char) = delete;
void printInt(bool) = delete;

template <typename T>
void foo(T) = delete;
void foo(int) {}

struct A {
    A() = delete;
    A(int) = delete;
    A(const A&) = delete;
    A& operator=(const A&) = delete;
    A& operator=(A&&) = delete;
    ~A() = delete;
    operator int() = delete;
};
)";

    const std::vector<std::string> deleted_constructs_cpp_srcml = {
        R"(<function_decl><type><name>void</name></type> <name>printInt</name><parameter_list>(<parameter><decl><type><name>char</name></type></decl></parameter>)</parameter_list> = <specifier>delete</specifier>;</function_decl>)",
        R"(<function_decl><type><name>void</name></type> <name>printInt</name><parameter_list>(<parameter><decl><type><name>bool</name></type></decl></parameter>)</parameter_list> = <specifier>delete</specifier>;</function_decl>)",
        R"(<function_decl><template>template <parameter_list>&lt;<parameter><type><name>typename</name></type> <name>T</name></parameter>&gt;</parameter_list></template>
<type><name>void</name></type> <name>foo</name><parameter_list>(<parameter><decl><type><name>T</name></type></decl></parameter>)</parameter_list> = <specifier>delete</specifier>;</function_decl>)",
        R"(<constructor_decl><name>A</name><parameter_list>()</parameter_list> = <specifier>delete</specifier>;</constructor_decl>)",
        R"(<constructor_decl><name>A</name><parameter_list>(<parameter><decl><type><name>int</name></type></decl></parameter>)</parameter_list> = <specifier>delete</specifier>;</constructor_decl>)",
        R"(<constructor_decl><name>A</name><parameter_list>(<parameter><decl><type><specifier>const</specifier> <name>A</name><modifier>&amp;</modifier></type></decl></parameter>)</parameter_list> = <specifier>delete</specifier>;</constructor_decl>)",
        R"(<function_decl type="operator"><type><name>A</name><modifier>&amp;</modifier></type> <name>operator<name>=</name></name><parameter_list>(<parameter><decl><type><specifier>const</specifier> <name>A</name><modifier>&amp;</modifier></type></decl></parameter>)</parameter_list> = <specifier>delete</specifier>;</function_decl>)",
        R"(<function_decl type="operator"><type><name>A</name><modifier>&amp;</modifier></type> <name>operator<name>=</name></name><parameter_list>(<parameter><decl><type><name>A</name><modifier>&amp;&amp;</modifier></type></decl></parameter>)</parameter_list> = <specifier>delete</specifier>;</function_decl>)",
        R"(<destructor_decl><name>~A</name><parameter_list>()</parameter_list> = <specifier>delete</specifier>;</destructor_decl>)",
        R"(<function_decl type="operator"><name>operator <name>int</name></name><parameter_list>()</parameter_list> = <specifier>delete</specifier>;</function_decl>)"
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,deleted_constructs_cpp.c_str(),deleted_constructs_cpp.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-deleted(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 10);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), deleted_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), deleted_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), deleted_constructs_cpp_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), deleted_constructs_cpp_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), deleted_constructs_cpp_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), deleted_constructs_cpp_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), deleted_constructs_cpp_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), deleted_constructs_cpp_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), deleted_constructs_cpp_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), deleted_constructs_cpp_srcml[9]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Function Exists check
    {
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::cout << srcml_get_language_list(i) << std::endl;
            char* s;
            size_t size;

            srcml_archive* oarchive = srcml_archive_create();
            srcml_archive_write_open_memory(oarchive,&s, &size);

            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,srcml_get_language_list(i));
            srcml_unit_parse_memory(unit,"",0);
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

            srcml_unit_free(unit);
            srcml_archive_close(oarchive);
            srcml_archive_free(oarchive);

            std::string srcml_text = std::string(s, size);
            free(s);

            srcml_archive* iarchive = srcml_archive_create();
            srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
            dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-deleted(.) or true()]"), SRCML_STATUS_OK);

            unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            dassert(srcml_unit_apply_transforms(iarchive, unit, &result), SRCML_STATUS_OK);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        }
    }


    //// is-const
    // C++
    const std::string const_constructs_cpp = R"(
const int x = 5;
const int x = 5, y = 10;
const int* p1;
int* const p2 = &x;
const int* const p3 = &x;

void f(const int x);
const int f();

class A {
public:
    const int x;
    int get() const;
    const int foo() const;
    const int* const bar(const int* const x) const;
};
)";

    const std::vector<std::string> const_constructs_cpp_srcml = {
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>, <decl><type ref="prev"/><name>y</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>)",
        R"(<decl><type ref="prev"/><name>y</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier></type> <name>p1</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier></type> <name>p1</name></decl>)",
        R"(<decl_stmt><decl><type><name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>p2</name> <init>= <expr><operator>&amp;</operator><name>x</name></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>p2</name> <init>= <expr><operator>&amp;</operator><name>x</name></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>p3</name> <init>= <expr><operator>&amp;</operator><name>x</name></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>p3</name> <init>= <expr><operator>&amp;</operator><name>x</name></expr></init></decl>)",
        R"(<parameter><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl></parameter>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl>)",
        R"(<function_decl><type><name>int</name></type> <name>get</name><parameter_list>()</parameter_list> <specifier>const</specifier>;</function_decl>)",
        R"(<function_decl><type><specifier>const</specifier> <name>int</name></type> <name>foo</name><parameter_list>()</parameter_list> <specifier>const</specifier>;</function_decl>)",
        R"(<function_decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>bar</name><parameter_list>(<parameter><decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>x</name></decl></parameter>)</parameter_list> <specifier>const</specifier>;</function_decl>)",
        R"(<parameter><decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>x</name></decl></parameter>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>x</name></decl>)",
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,const_constructs_cpp.c_str(),const_constructs_cpp.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-const(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 20);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), const_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), const_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), const_constructs_cpp_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), const_constructs_cpp_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), const_constructs_cpp_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), const_constructs_cpp_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), const_constructs_cpp_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), const_constructs_cpp_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), const_constructs_cpp_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), const_constructs_cpp_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), const_constructs_cpp_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), const_constructs_cpp_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), const_constructs_cpp_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), const_constructs_cpp_srcml[13]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), const_constructs_cpp_srcml[14]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), const_constructs_cpp_srcml[15]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), const_constructs_cpp_srcml[16]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), const_constructs_cpp_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), const_constructs_cpp_srcml[18]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), const_constructs_cpp_srcml[19]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // C#
    const std::string const_constructs_cs = R"(
class A {
    public const int X = 5;
    private const string Name = "test";
    void f() {
        const int x = 10;
    }
}
)";

    const std::vector<std::string> const_constructs_cs_srcml = {
        R"(<decl_stmt><decl><type><specifier>public</specifier> <specifier>const</specifier> <name>int</name></type> <name>X</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>public</specifier> <specifier>const</specifier> <name>int</name></type> <name>X</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>private</specifier> <specifier>const</specifier> <name>string</name></type> <name>Name</name> <init>= <expr><literal type="string">"test"</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>private</specifier> <specifier>const</specifier> <name>string</name></type> <name>Name</name> <init>= <expr><literal type="string">"test"</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>)"
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,const_constructs_cs.c_str(),const_constructs_cs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-const(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 6);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), const_constructs_cs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), const_constructs_cs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), const_constructs_cs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), const_constructs_cs_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), const_constructs_cs_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), const_constructs_cs_srcml[5]);

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }

    // Function Exists check
    {
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::cout << srcml_get_language_list(i) << std::endl;
            char* s;
            size_t size;

            srcml_archive* oarchive = srcml_archive_create();
            srcml_archive_write_open_memory(oarchive,&s, &size);

            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,srcml_get_language_list(i));
            srcml_unit_parse_memory(unit,"",0);
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

            srcml_unit_free(unit);
            srcml_archive_close(oarchive);
            srcml_archive_free(oarchive);

            std::string srcml_text = std::string(s, size);
            free(s);

            srcml_archive* iarchive = srcml_archive_create();
            srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
            dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-deleted(.) or true()]"), SRCML_STATUS_OK);

            unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            dassert(srcml_unit_apply_transforms(iarchive, unit, &result), SRCML_STATUS_OK);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        }
    }



    //// is-constant
    // C++
    const std::string constant_constructs_cpp = R"(
const int x = 5;
const int x = 5, y = 10;
const int* p1;
int* const p2 = &x;
const int* const p3 = &x;

void f(const int x);
const int f();

class A {
public:
    const int x;
    int get() const;
    const int foo() const;
    const int* const bar(const int* const x) const;
};

constexpr int x = 10;
constexpr int square(int x) { return x * x; }

class conststr
{
    constexpr conststr() {}
    constexpr conststr(int);
    constexpr char operator[](std::size_t n) const {}
    constexpr std::size_t size() const { return sz; }
};

)";

    const std::vector<std::string> constant_constructs_cpp_srcml = {
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>, <decl><type ref="prev"/><name>y</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>)",
        R"(<decl><type ref="prev"/><name>y</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier></type> <name>p1</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier></type> <name>p1</name></decl>)",
        R"(<decl_stmt><decl><type><name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>p2</name> <init>= <expr><operator>&amp;</operator><name>x</name></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>p2</name> <init>= <expr><operator>&amp;</operator><name>x</name></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>p3</name> <init>= <expr><operator>&amp;</operator><name>x</name></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>p3</name> <init>= <expr><operator>&amp;</operator><name>x</name></expr></init></decl>)",
        R"(<parameter><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl></parameter>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl>)",
        R"(<function_decl><type><name>int</name></type> <name>get</name><parameter_list>()</parameter_list> <specifier>const</specifier>;</function_decl>)",
        R"(<function_decl><type><specifier>const</specifier> <name>int</name></type> <name>foo</name><parameter_list>()</parameter_list> <specifier>const</specifier>;</function_decl>)",
        R"(<function_decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>bar</name><parameter_list>(<parameter><decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>x</name></decl></parameter>)</parameter_list> <specifier>const</specifier>;</function_decl>)",
        R"(<parameter><decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>x</name></decl></parameter>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name><modifier>*</modifier> <specifier>const</specifier></type> <name>x</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>constexpr</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>constexpr</specifier> <name>int</name></type> <name>x</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>)",
        R"(<function><type><specifier>constexpr</specifier> <name>int</name></type> <name>square</name><parameter_list>(<parameter><decl><type><name>int</name></type> <name>x</name></decl></parameter>)</parameter_list> <block>{<block_content> <return>return <expr><name>x</name> <operator>*</operator> <name>x</name></expr>;</return> </block_content>}</block></function>)",
        R"(<constructor><specifier>constexpr</specifier> <name>conststr</name><parameter_list>()</parameter_list> <block>{<block_content/>}</block></constructor>)",
        R"(<constructor_decl><specifier>constexpr</specifier> <name>conststr</name><parameter_list>(<parameter><decl><type><name>int</name></type></decl></parameter>)</parameter_list>;</constructor_decl>)",
        R"(<function type="operator"><type><specifier>constexpr</specifier> <name>char</name></type> <name>operator<name>[]</name></name><parameter_list>(<parameter><decl><type><name><name>std</name><operator>::</operator><name>size_t</name></name></type> <name>n</name></decl></parameter>)</parameter_list> <specifier>const</specifier> <block>{<block_content/>}</block></function>)",
        R"(<function><type><specifier>constexpr</specifier> <name><name>std</name><operator>::</operator><name>size_t</name></name></type> <name>size</name><parameter_list>()</parameter_list> <specifier>const</specifier> <block>{<block_content> <return>return <expr><name>sz</name></expr>;</return> </block_content>}</block></function>)",
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C++");
        srcml_unit_parse_memory(unit,constant_constructs_cpp.c_str(),constant_constructs_cpp.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-constant(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 27);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), constant_constructs_cpp_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), constant_constructs_cpp_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), constant_constructs_cpp_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), constant_constructs_cpp_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), constant_constructs_cpp_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), constant_constructs_cpp_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), constant_constructs_cpp_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), constant_constructs_cpp_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), constant_constructs_cpp_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), constant_constructs_cpp_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), constant_constructs_cpp_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), constant_constructs_cpp_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), constant_constructs_cpp_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), constant_constructs_cpp_srcml[13]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), constant_constructs_cpp_srcml[14]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), constant_constructs_cpp_srcml[15]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), constant_constructs_cpp_srcml[16]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), constant_constructs_cpp_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), constant_constructs_cpp_srcml[18]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), constant_constructs_cpp_srcml[19]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), constant_constructs_cpp_srcml[20]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,21)), constant_constructs_cpp_srcml[21]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,22)), constant_constructs_cpp_srcml[22]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,23)), constant_constructs_cpp_srcml[23]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,24)), constant_constructs_cpp_srcml[24]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,25)), constant_constructs_cpp_srcml[25]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,26)), constant_constructs_cpp_srcml[26]);
        

        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }


    // C#
    const std::string constant_constructs_cs = R"(
const int a = 5;
const int b = a + 10;
readonly int x;
static readonly int x = ComputeValue();

readonly struct Point {
    public readonly int X;
    public readonly int Y;
}

void Foo(in int x) {}

const int a = 5, c = 10;
readonly int x, y;
)";

    const std::vector<std::string> constant_constructs_cs_srcml = {
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>a</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>a</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>b</name> <init>= <expr><name>a</name> <operator>+</operator> <literal type="number">10</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>b</name> <init>= <expr><name>a</name> <operator>+</operator> <literal type="number">10</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>readonly</specifier> <name>int</name></type> <name>x</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>readonly</specifier> <name>int</name></type> <name>x</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>static</specifier> <specifier>readonly</specifier> <name>int</name></type> <name>x</name> <init>= <expr><call><name>ComputeValue</name><argument_list>()</argument_list></call></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>static</specifier> <specifier>readonly</specifier> <name>int</name></type> <name>x</name> <init>= <expr><call><name>ComputeValue</name><argument_list>()</argument_list></call></expr></init></decl>)",
        R"(<struct><specifier>readonly</specifier> struct <name>Point</name> <block>{
    <decl_stmt><decl><type><specifier>public</specifier> <specifier>readonly</specifier> <name>int</name></type> <name>X</name></decl>;</decl_stmt>
    <decl_stmt><decl><type><specifier>public</specifier> <specifier>readonly</specifier> <name>int</name></type> <name>Y</name></decl>;</decl_stmt>
}</block></struct>)",
        R"(<decl_stmt><decl><type><specifier>public</specifier> <specifier>readonly</specifier> <name>int</name></type> <name>X</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>public</specifier> <specifier>readonly</specifier> <name>int</name></type> <name>X</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>public</specifier> <specifier>readonly</specifier> <name>int</name></type> <name>Y</name></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>public</specifier> <specifier>readonly</specifier> <name>int</name></type> <name>Y</name></decl>)",
        R"(<parameter><decl><type><specifier>in</specifier> <name>int</name></type> <name>x</name></decl></parameter>)",
        R"(<decl><type><specifier>in</specifier> <name>int</name></type> <name>x</name></decl>)",
        R"(<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>a</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>, <decl><type ref="prev"/><name>c</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>;</decl_stmt>)",
        R"(<decl><type><specifier>const</specifier> <name>int</name></type> <name>a</name> <init>= <expr><literal type="number">5</literal></expr></init></decl>)",
        R"(<decl><type ref="prev"/><name>c</name> <init>= <expr><literal type="number">10</literal></expr></init></decl>)",
        R"(<decl_stmt><decl><type><specifier>readonly</specifier> <name>int</name></type> <name>x</name></decl>, <decl><type ref="prev"/><name>y</name></decl>;</decl_stmt></unit>)",
        R"(<decl><type><specifier>readonly</specifier> <name>int</name></type> <name>x</name></decl>)",
        R"(<decl><type ref="prev"/><name>y</name></decl>)",
    };

    {
        char* s;
        size_t size;

        srcml_archive* oarchive = srcml_archive_create();
        srcml_archive_write_open_memory(oarchive,&s, &size);

        srcml_unit* unit = srcml_unit_create(oarchive);
        srcml_unit_set_language(unit,"C#");
        srcml_unit_parse_memory(unit,constant_constructs_cs.c_str(),constant_constructs_cs.size());
        dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(oarchive);
        srcml_archive_free(oarchive);

        std::string srcml_text = std::string(s, size);
        free(s);

        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
        dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-constant(.)]"), SRCML_STATUS_OK);

        unit = srcml_archive_read_unit(iarchive);
        srcml_transform_result* result = nullptr;
        srcml_unit_apply_transforms(iarchive, unit, &result);

        dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        dassert(srcml_transform_get_unit_size(result), 21);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,0)), constant_constructs_cs_srcml[0]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,1)), constant_constructs_cs_srcml[1]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,2)), constant_constructs_cs_srcml[2]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,3)), constant_constructs_cs_srcml[3]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,4)), constant_constructs_cs_srcml[4]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,5)), constant_constructs_cs_srcml[5]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,6)), constant_constructs_cs_srcml[6]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,7)), constant_constructs_cs_srcml[7]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,8)), constant_constructs_cs_srcml[8]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,9)), constant_constructs_cs_srcml[9]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,10)), constant_constructs_cs_srcml[10]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,11)), constant_constructs_cs_srcml[11]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,12)), constant_constructs_cs_srcml[12]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,13)), constant_constructs_cs_srcml[13]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,14)), constant_constructs_cs_srcml[14]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,15)), constant_constructs_cs_srcml[15]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,16)), constant_constructs_cs_srcml[16]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,17)), constant_constructs_cs_srcml[17]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,18)), constant_constructs_cs_srcml[18]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,19)), constant_constructs_cs_srcml[19]);
        dassert(srcml_unit_get_srcml_inner(srcml_transform_get_unit(result,20)), constant_constructs_cs_srcml[20]);


        srcml_unit_free(unit);
        srcml_transform_free(result);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
    }




    // Function Exists check
    {
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::cout << srcml_get_language_list(i) << std::endl;
            char* s;
            size_t size;

            srcml_archive* oarchive = srcml_archive_create();
            srcml_archive_write_open_memory(oarchive,&s, &size);

            srcml_unit* unit = srcml_unit_create(oarchive);
            srcml_unit_set_language(unit,srcml_get_language_list(i));
            srcml_unit_parse_memory(unit,"",0);
            dassert(srcml_archive_write_unit(oarchive,unit), SRCML_STATUS_OK);

            srcml_unit_free(unit);
            srcml_archive_close(oarchive);
            srcml_archive_free(oarchive);

            std::string srcml_text = std::string(s, size);
            free(s);

            srcml_archive* iarchive = srcml_archive_create();
            srcml_archive_read_open_memory(iarchive,srcml_text.c_str(),srcml_text.size());
            dassert(srcml_append_transform_xpath(iarchive,"//*[src:is-constant(.) or true()]"), SRCML_STATUS_OK);

            unit = srcml_archive_read_unit(iarchive);
            srcml_transform_result* result = nullptr;
            dassert(srcml_unit_apply_transforms(iarchive, unit, &result), SRCML_STATUS_OK);

            dassert(srcml_transform_get_type(result), SRCML_RESULT_UNITS);
        }
    }
}
