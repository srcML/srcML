#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define schema <<- 'STDOUT'
	<grammar xmlns="http://relaxng.org/ns/structure/1.0">

	  <start>
	    <ref name="anyElement"/>
	  </start>

	  <define name="anyElement">
	    <element>
	      <anyName/>
	      <zeroOrMore>
	      <choice>
	        <attribute>
		    <anyName/>
		      </attribute>
		        <text/>
			  <ref name="anyElement"/>
			  </choice>
	      </zeroOrMore>
	    </element>
	  </define>

	</grammar>
	STDOUT


# test on archive of many
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" hash="17239ee1ffe4b0abc790a338f52d2ac2f2565c4e"><function_decl><type><name>LIBSRCML_DECL</name> <name>int</name></type> <name>srcml_check_xslt</name><parameter_list>()</parameter_list>;</function_decl>
	</unit>

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

xmlcheck "$schema"
xmlcheck "$srcml"

createfile schema.rng "$schema"
createfile sub/archive_multi.xml "$srcml"

# apply schema to archive of many units
srcml schema.rng sub/archive_multi.xml
check 3<<< "$srcml"

srcml schema.rng < sub/archive_multi.xml
check 3<<< "$srcml"

srcml schema.rng sub/archive_multi.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml schema.rng -o sub/b.cpp.xml sub/archive_multi.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml schema.rng -o sub/b.cpp.xml < sub/archive_multi.xml
check sub/b.cpp.xml 3<<< "$srcml"

# apply root and apply schema
srcml schema.rng sub/archive_multi.xml
check 3<<< "$srcml"

srcml schema.rng < sub/archive_multi.xml
check 3<<< "$srcml"

srcml schema.rng sub/archive_multi.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml schema.rng -o sub/b.cpp.xml sub/archive_multi.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml schema.rng -o sub/b.cpp.xml < sub/archive_multi.xml
check sub/b.cpp.xml 3<<< "$srcml"


# test on archive of one
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" hash="17239ee1ffe4b0abc790a338f52d2ac2f2565c4e"><function_decl><type><name>LIBSRCML_DECL</name> <name>int</name></type> <name>srcml_check_xslt</name><parameter_list>()</parameter_list>;</function_decl>
	</unit>

	</unit>
	STDOUT

createfile sub/archive_single.xml "$srcml"

# apply schema
srcml schema.rng sub/archive_single.xml
check 3<<< "$srcml"

srcml schema.rng < sub/archive_single.xml
check 3<<< "$srcml"

srcml schema.rng sub/archive_single.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml schema.rng -o sub/b.cpp.xml sub/archive_single.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml schema.rng -o sub/b.cpp.xml < sub/archive_single.xml
check sub/b.cpp.xml 3<<< "$srcml"

# apply root and apply schema
srcml schema.rng sub/archive_single.xml
check 3<<< "$srcml"

srcml schema.rng < sub/archive_single.xml
check 3<<< "$srcml"

srcml schema.rng sub/archive_single.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml schema.rng -o sub/b.cpp.xml sub/archive_single.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml schema.rng -o sub/b.cpp.xml < sub/archive_single.xml
check sub/b.cpp.xml 3<<< "$srcml"

