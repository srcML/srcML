#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test enabling hash
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$fsrcml"
xmlcheck "$srcml"
createfile sub/a.cpp "a;"

# from a file
src2srcml sub/a.cpp --hash
checkv2 "$fsrcml"

src2srcml --hash sub/a.cpp
checkv2 "$fsrcml"

src2srcml sub/a.cpp --hash -o sub/a.xml
checkv2 sub/a.xml "$fsrcml"

src2srcml sub/a.cpp -o sub/a.xml --hash
checkv2 sub/a.xml "$fsrcml"

src2srcml -o sub/a.xml sub/a.cpp --hash
checkv2 sub/a.xml "$fsrcml"

src2srcml --hash sub/a.cpp -o sub/a.xml
checkv2 sub/a.xml "$fsrcml"

src2srcml --hash -o sub/a.xml sub/a.cpp
checkv2 sub/a.xml "$fsrcml"

src2srcml -o sub/a.xml --hash sub/a.cpp
checkv2 sub/a.xml "$fsrcml"

# standard input
src2srcml -l C++ --hash < sub/a.cpp
checkv2 "$srcml"

src2srcml --hash -l C++ < sub/a.cpp
checkv2 "$srcml"

src2srcml -l C++ --hash -o sub/a.xml < sub/a.cpp
checkv2 sub/a.xml "$srcml"

src2srcml -l C++ -o sub/a.xml --hash < sub/a.cpp
checkv2 sub/a.xml "$srcml"

src2srcml -o sub/a.xml -l C++ --hash < sub/a.cpp
checkv2 sub/a.xml "$srcml"

src2srcml --hash -l C++ -o sub/a.xml < sub/a.cpp
checkv2 sub/a.xml "$srcml"

src2srcml --hash -o sub/a.xml -l C++ < sub/a.cpp
checkv2 sub/a.xml "$srcml"

src2srcml -o sub/a.xml --hash -l C++ < sub/a.cpp
checkv2 sub/a.xml "$srcml"
