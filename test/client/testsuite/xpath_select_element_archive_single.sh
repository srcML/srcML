#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test executing an xpath search on an archive with more than one unit in it
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="c671c9b9a1bc97b6902a611a98700718006bd736"><comment type="block" format="doxygen">/**
	 * @returns Return 1 on success and 0 on failure.
	 */</comment>
	<function><type><name>int</name></type> <name>srcml_check_xslt</name><parameter_list>()</parameter_list> <block>{<block_content>
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><call><name>defined</name><argument_list>(<argument><expr><name>__GNUG__</name></expr></argument>)</argument_list></call> <operator>&amp;&amp;</operator> <operator>!</operator><call><name>defined</name><argument_list>(<argument><expr><name>__MINGW32__</name></expr></argument>)</argument_list></call> <operator>&amp;&amp;</operator> <operator>!</operator><call><name>defined</name><argument_list>(<argument><expr><name>NO_DLLOAD</name></expr></argument>)</argument_list></call></expr></cpp:if>
	  <decl_stmt><decl><type><name>void</name> <modifier>*</modifier></type> <name>handle</name> <init>= <expr><call><name>dlopen</name><argument_list>(<argument><expr><literal type="string">"libxslt.so"</literal></expr></argument>, <argument><expr><name>RTLD_LAZY</name></expr></argument>)</argument_list></call></expr></init></decl>;</decl_stmt>
	  <if_stmt><if>if <condition>(<expr><operator>!</operator><name>handle</name></expr>)</condition> <block>{<block_content>
	    <expr_stmt><expr><name>handle</name> <operator>=</operator> <call><name>dlopen</name><argument_list>(<argument><expr><literal type="string">"libxslt.so.1"</literal></expr></argument>, <argument><expr><name>RTLD_LAZY</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>
	    <if_stmt><if>if <condition>(<expr><operator>!</operator><name>handle</name></expr>)</condition> <block>{<block_content>
	      <expr_stmt><expr><name>handle</name> <operator>=</operator> <call><name>dlopen</name><argument_list>(<argument><expr><literal type="string">"libxslt.dylib"</literal></expr></argument>, <argument><expr><name>RTLD_LAZY</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>
	      <if_stmt><if>if <condition>(<expr><operator>!</operator><name>handle</name></expr>)</condition><block type="pseudo"><block_content> <return>return <expr><literal type="number">0</literal></expr>;</return></block_content></block></if></if_stmt>
	    </block_content>}</block></if></if_stmt>
	  </block_content>}</block></if></if_stmt>
	  <expr_stmt><expr><call><name>dlclose</name><argument_list>(<argument><expr><name>handle</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>
	  <return>return <expr><literal type="number">1</literal></expr>;</return>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	  <return>return <expr><literal type="number">1</literal></expr>;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</block_content>}</block></function>
	</unit>

	</unit>
	STDOUT

xmlcheck "$srcml"
createfile sub/archive.cpp.xml "$srcml"

# select elements matching *<argument><expr><name>*
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" item="1"><name>__GNUG__</name></unit>

	<unit revision="REVISION" language="C++" filename="a.cpp" item="2"><name>__MINGW32__</name></unit>

	<unit revision="REVISION" language="C++" filename="a.cpp" item="3"><name>NO_DLLOAD</name></unit>

	<unit revision="REVISION" language="C++" filename="a.cpp" item="4"><name>RTLD_LAZY</name></unit>

	<unit revision="REVISION" language="C++" filename="a.cpp" item="5"><name>RTLD_LAZY</name></unit>

	<unit revision="REVISION" language="C++" filename="a.cpp" item="6"><name>RTLD_LAZY</name></unit>

	<unit revision="REVISION" language="C++" filename="a.cpp" item="7"><name>handle</name></unit>

	</unit>
	STDOUT

xmlcheck "$output"

srcml sub/archive.cpp.xml --xpath "//src:argument/src:expr/src:name"
check "$output"

srcml --xpath "//src:argument/src:expr/src:name" sub/archive.cpp.xml
check "$output"

srcml sub/archive.cpp.xml --xpath "//src:argument/src:expr/src:name" -o sub/a.xml
check sub/a.xml "$output"

srcml --xpath "//src:argument/src:expr/src:name" sub/archive.cpp.xml -o sub/a.xml
check sub/a.xml "$output"

srcml sub/archive.cpp.xml -o sub/a.xml --xpath "//src:argument/src:expr/src:name"
check sub/a.xml "$output"

srcml -o sub/a.xml sub/archive.cpp.xml --xpath "//src:argument/src:expr/src:name"
check sub/a.xml "$output"

# select the first elements (from each archive unit) that match *<name>*
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" item="1"><name>int</name></unit>

	</unit>
	STDOUT

xmlcheck "$output"

srcml sub/archive.cpp.xml --xpath "(//src:name)[1]"
check "$output"

srcml --xpath "(//src:name)[1]" sub/archive.cpp.xml
check "$output"

srcml sub/archive.cpp.xml --xpath "(//src:name)[1]" -o sub/a.xml
check sub/a.xml "$output"

srcml sub/archive.cpp.xml -o sub/a.xml --xpath "(//src:name)[1]"
check sub/a.xml "$output"

srcml -o sub/a.xml sub/archive.cpp.xml --xpath "(//src:name)[1]"
check sub/a.xml "$output"

srcml -o sub/a.xml --xpath "(//src:name)[1]" sub/archive.cpp.xml
check sub/a.xml "$output"

srcml --xpath "(//src:name)[1]" -o sub/a.xml sub/archive.cpp.xml
check sub/a.xml "$output"

# select the text from *<type><name>
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" item="1">int</unit>

	<unit revision="REVISION" language="C++" filename="a.cpp" item="2">void</unit>

	</unit>
	STDOUT

xmlcheck "$output"

srcml sub/archive.cpp.xml --xpath "//src:type/src:name/text()"
check "$output"

srcml --xpath "//src:type/src:name/text()" sub/archive.cpp.xml
check "$output"

srcml sub/archive.cpp.xml --xpath "//src:type/src:name/text()" -o sub/a.xml
check sub/a.xml "$output"

srcml sub/archive.cpp.xml -o sub/a.xml --xpath "//src:type/src:name/text()"
check sub/a.xml "$output"

srcml -o sub/a.xml sub/archive.cpp.xml --xpath "//src:type/src:name/text()"
check sub/a.xml "$output"

srcml -o sub/a.xml --xpath "//src:type/src:name/text()" sub/archive.cpp.xml
check sub/a.xml "$output"

srcml --xpath "//src:type/src:name/text()" -o sub/a.xml sub/archive.cpp.xml
check sub/a.xml "$output"

srcml --xpath "//src:type/src:name/text()" sub/archive.cpp.xml -o sub/a.xml
check sub/a.xml "$output"

# select all comment elements that contain block comments
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" item="1"><comment type="block" format="doxygen">/**
	 * @returns Return 1 on success and 0 on failure.
	 */</comment></unit>

	</unit>
	STDOUT

xmlcheck "$output"

srcml sub/archive.cpp.xml --xpath='//src:comment[@type="block"]'
check "$output"

srcml --xpath='//src:comment[@type="block"]' sub/archive.cpp.xml
check "$output"

srcml sub/archive.cpp.xml --xpath='//src:comment[@type="block"]' -o sub/a.xml
check sub/a.xml "$output"

srcml sub/archive.cpp.xml -o sub/a.xml --xpath='//src:comment[@type="block"]'
check sub/a.xml "$output"

srcml -o sub/a.xml sub/archive.cpp.xml --xpath='//src:comment[@type="block"]'
check sub/a.xml "$output"

srcml -o sub/a.xml --xpath='//src:comment[@type="block"]' sub/archive.cpp.xml
check sub/a.xml "$output"

srcml --xpath='//src:comment[@type="block"]' -o sub/a.xml sub/archive.cpp.xml
check sub/a.xml "$output"

srcml --xpath='//src:comment[@type="block"]' sub/archive.cpp.xml -o sub/a.xml
check sub/a.xml "$output"
