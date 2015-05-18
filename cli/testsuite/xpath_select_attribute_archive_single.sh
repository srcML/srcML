#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test xpath query for attribute info
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="c671c9b9a1bc97b6902a611a98700718006bd736"><comment type="block" format="doxygen">/**
	 * @returns Return 1 on success and 0 on failure.
	 */</comment>
	<function><type><name>int</name></type> <name>srcml_check_xslt</name><parameter_list>()</parameter_list> <block>{
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><call><name>defined</name><argument_list>(<argument><expr><name>__GNUG__</name></expr></argument>)</argument_list></call> <operator>&amp;&amp;</operator> <operator>!</operator><call><name>defined</name><argument_list>(<argument><expr><name>__MINGW32__</name></expr></argument>)</argument_list></call> <operator>&amp;&amp;</operator> <operator>!</operator><call><name>defined</name><argument_list>(<argument><expr><name>NO_DLLOAD</name></expr></argument>)</argument_list></call></expr></cpp:if>
	  <decl_stmt><decl><type><name>void</name> <modifier>*</modifier></type> <name>handle</name> <init>= <expr><call><name>dlopen</name><argument_list>(<argument><expr><literal type="string">"libxslt.so"</literal></expr></argument>, <argument><expr><name>RTLD_LAZY</name></expr></argument>)</argument_list></call></expr></init></decl>;</decl_stmt>
	  <if>if <condition>(<expr><operator>!</operator><name>handle</name></expr>)</condition><then> <block>{
	    <expr_stmt><expr><name>handle</name> <operator>=</operator> <call><name>dlopen</name><argument_list>(<argument><expr><literal type="string">"libxslt.so.1"</literal></expr></argument>, <argument><expr><name>RTLD_LAZY</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>
	    <if>if <condition>(<expr><operator>!</operator><name>handle</name></expr>)</condition><then> <block>{
	      <expr_stmt><expr><name>handle</name> <operator>=</operator> <call><name>dlopen</name><argument_list>(<argument><expr><literal type="string">"libxslt.dylib"</literal></expr></argument>, <argument><expr><name>RTLD_LAZY</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>
	      <if>if <condition>(<expr><operator>!</operator><name>handle</name></expr>)</condition><then> <block type="pseudo"><return>return <expr><literal type="number">0</literal></expr>;</return></block></then></if>
	    }</block></then></if>
	  }</block></then></if>
	  <expr_stmt><expr><call><name>dlclose</name><argument_list>(<argument><expr><name>handle</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>
	  <return>return <expr><literal type="number">1</literal></expr>;</return>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	  <return>return <expr><literal type="number">1</literal></expr>;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	}</block></function>
	</unit>

	</unit>
	STDOUT

createfile sub/archive_single.cpp.xml "$srcml"


# select filename
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" item="1">filename="a.cpp"</unit>

	</unit>
	STDOUT

srcml sub/archive_single.cpp.xml --xpath "//src:unit/@filename"
check 3<<< "$output"

srcml --xpath "//src:unit/@filename" sub/archive_single.cpp.xml
check 3<<< "$output"

srcml sub/archive_single.cpp.xml --xpath "//src:unit/@filename" -o sub/a.xml
check sub/a.xml 3<<< "$output"

srcml sub/archive_single.cpp.xml -o sub/a.xml --xpath "//src:unit/@filename"
check sub/a.xml 3<<< "$output"

srcml -o sub/a.xml sub/archive_single.cpp.xml --xpath "//src:unit/@filename"
check sub/a.xml 3<<< "$output"

srcml --xpath "//src:unit/@filename" sub/archive_single.cpp.xml -o sub/a.xml
check sub/a.xml 3<<< "$output"

srcml --xpath "//src:unit/@filename" sub/archive_single.cpp.xml
check sub/a.xml 3<<< "$output"

srcml --xpath "//src:unit/@filename" sub/archive_single.cpp.xml
check sub/a.xml 3<<< "$output"


# select comment format (doxygen)
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" item="1">format="doxygen"</unit>

	</unit>
	STDOUT

srcml sub/archive_single.cpp.xml --xpath "//src:comment/@format"
check 3<<< "$output"

srcml --xpath "//src:comment/@format" sub/archive_single.cpp.xml
check 3<<< "$output"

srcml sub/archive_single.cpp.xml --xpath "//src:comment/@format" -o sub/a.xml
check sub/a.xml 3<<< "$output"

srcml sub/archive_single.cpp.xml -o sub/a.xml --xpath "//src:comment/@format"
check sub/a.xml 3<<< "$output"

srcml -o sub/a.xml sub/archive_single.cpp.xml --xpath "//src:comment/@format"
check sub/a.xml 3<<< "$output"

srcml --xpath "//src:comment/@format" sub/archive_single.cpp.xml -o sub/a.xml
check sub/a.xml 3<<< "$output"

srcml --xpath "//src:comment/@format" sub/archive_single.cpp.xml
check sub/a.xml 3<<< "$output"

srcml --xpath "//src:comment/@format" sub/archive_single.cpp.xml
check sub/a.xml 3<<< "$output"


