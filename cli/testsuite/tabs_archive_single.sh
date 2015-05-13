#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="8">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="8">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""

# specify tab size of 8
srcml -l C++ --tabs 8 --archive < sub/a.cpp
check 3<<< "$srcml"

srcml -l C++ --archive --tabs 8 < sub/a.cpp
check 3<<< "$srcml"

srcml --archive -l C++ --tabs 8 < sub/a.cpp
check 3<<< "$srcml"

srcml --tabs 8 --archive sub/a.cpp
check 3<<< "$fsrcml"

srcml --archive --tabs 8 sub/a.cpp
check 3<<< "$fsrcml"

srcml --tabs 8 sub/a.cpp --archive
check 3<<< "$fsrcml"

srcml sub/a.cpp --archive --tabs 8
check 3<<< "$fsrcml"

srcml -l C++ --tabs 8 -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ -o sub/a.cpp.xml --tabs 8 --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -o sub/a.cpp.xml --tabs 8 -l C++ --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ --tabs 8 --archive -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml  --archive -l C++ -o sub/a.cpp.xml --tabs 8 < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml --tabs 8 sub/a.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 sub/a.cpp --archive -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 8 sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 --archive sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 -o sub/a.cpp.xml sub/a.cpp --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 -o sub/a.cpp.xml --archive sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 --archive -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 8 -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

# use default tab size of 8
srcml -l C++ --tabs --archive < sub/a.cpp
check 3<<< "$srcml"

srcml -l C++ --archive --tabs < sub/a.cpp
check 3<<< "$srcml"

srcml --archive -l C++ --tabs < sub/a.cpp
check 3<<< "$srcml"

srcml --tabs --archive sub/a.cpp
check 3<<< "$fsrcml"

srcml sub/a.cpp --archive --tabs
check 3<<< "$fsrcml"

srcml -l C++ --tabs -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ -o sub/a.cpp.xml --tabs --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -o sub/a.cpp.xml --tabs -l C++ --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ --tabs --archive -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml  --archive -l C++ -o sub/a.cpp.xml --tabs < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml --tabs --archive sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs -o sub/a.cpp.xml sub/a.cpp --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs -o sub/a.cpp.xml --archive sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs --archive -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"
