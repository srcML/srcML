#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test --cpp-nomarkup-else
define input <<- 'INPUT'
	
	#if A
	break;
	#else
	return;
	#endif
	INPUT

define fmarkup_else <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="ece2a60901d6ccdfe670f0f7445560a55dac57a6">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define fnomarkup_else <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="ece2a60901d6ccdfe670f0f7445560a55dac57a6">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	return;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT


createfile sub/a.cpp "$input"
createfile sub/b.cpp "b;"

# markup else by default
src2srcml sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fmarkup_else"

src2srcml sub/a.cpp sub/b.cpp -o sub/b.cpp.xml --in-order
check sub/b.cpp.xml 3<<< "$fmarkup_else"

src2srcml -o sub/b.cpp.xml sub/a.cpp sub/b.cpp --in-order
check sub/b.cpp.xml 3<<< "$fmarkup_else"

# don't markup else
src2srcml sub/a.cpp sub/b.cpp --cpp-nomarkup-else --in-order
check 3<<< "$fnomarkup_else"

src2srcml sub/a.cpp sub/b.cpp --cpp-nomarkup-else -o sub/b.cpp.xml --in-order
check sub/b.cpp.xml 3<<< "$fnomarkup_else"

src2srcml sub/a.cpp sub/b.cpp -o sub/b.cpp.xml --cpp-nomarkup-else --in-order
check sub/b.cpp.xml 3<<< "$fnomarkup_else"

src2srcml --cpp-nomarkup-else sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fnomarkup_else"

src2srcml --cpp-nomarkup-else sub/a.cpp sub/b.cpp -o sub/b.cpp.xml --in-order
check sub/b.cpp.xml 3<<< "$fnomarkup_else"

src2srcml --cpp-nomarkup-else -o sub/b.cpp.xml sub/a.cpp sub/b.cpp --in-order
check sub/b.cpp.xml 3<<< "$fnomarkup_else"
