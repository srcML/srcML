#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# no xml declaration on a single unit
define srcml <<- 'STDOUT'
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

define fsrcml <<- 'STDOUT'
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

createfile sub/a.cpp "a;"

# from standard in
srcml --no-xml-declaration -l C++ < sub/a.cpp
check 3<<< "$srcml"

srcml -l C++ --no-xml-declaration < sub/a.cpp
check 3<<< "$srcml"

srcml --no-xml-declaration -l C++ -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml --no-xml-declaration -o sub/a.cpp.xml -l C++ < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -o sub/a.cpp.xml --no-xml-declaration -l C++ < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ --no-xml-declaration -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ -o sub/a.cpp.xml --no-xml-declaration < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -o sub/a.cpp.xml -l C++ --no-xml-declaration < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

# from a file
srcml sub/a.cpp --no-xml-declaration
check 3<<< "$fsrcml"

srcml --no-xml-declaration sub/a.cpp
check 3<<< "$fsrcml"

srcml sub/a.cpp --no-xml-declaration -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --no-xml-declaration sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --no-xml-declaration -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml sub/a.cpp -o sub/a.cpp.xml --no-xml-declaration
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml -o sub/a.cpp.xml --no-xml-declaration sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml -o sub/a.cpp.xml sub/a.cpp --no-xml-declaration
check sub/a.cpp.xml 3<<< "$fsrcml"

