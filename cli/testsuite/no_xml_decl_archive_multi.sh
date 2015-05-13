#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# no xml declaration on a single unit
define srcml <<- 'STDOUT'
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

# from a file
srcml sub/a.cpp sub/b.cpp --in-order --no-xml-declaration
check 3<<< "$srcml"

srcml sub/a.cpp --in-order sub/b.cpp --no-xml-declaration
check 3<<< "$srcml"

srcml --no-xml-declaration sub/a.cpp sub/b.cpp --in-order
check 3<<< "$srcml"

srcml --no-xml-declaration sub/a.cpp --in-order sub/b.cpp
check 3<<< "$srcml"

srcml --no-xml-declaration --in-order sub/a.cpp sub/b.cpp
check 3<<< "$srcml"

srcml --in-order --no-xml-declaration sub/a.cpp sub/b.cpp
check 3<<< "$srcml"

srcml sub/a.cpp sub/b.cpp --in-order --no-xml-declaration -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

srcml --no-xml-declaration sub/a.cpp sub/b.cpp --in-order -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

srcml --no-xml-declaration -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$srcml"

srcml sub/a.cpp sub/b.cpp --in-order -o sub/a.cpp.xml --no-xml-declaration
check sub/a.cpp.xml 3<<< "$srcml"

srcml -o sub/a.cpp.xml --no-xml-declaration sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$srcml"

srcml -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order --no-xml-declaration
check sub/a.cpp.xml 3<<< "$srcml"

