#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="4">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="4" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="4">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" pos:tabs="4" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""

srcml -l C++ --tabs 4 --position --archive < sub/a.cpp
check 3<<< "$srcml"

srcml -l C++ --position --tabs 4 --archive < sub/a.cpp
check 3<<< "$srcml"

srcml -l C++ --archive  --position --tabs 4 < sub/a.cpp
check 3<<< "$srcml"

srcml -l C++ --archive --tabs 4 --position < sub/a.cpp
check 3<<< "$srcml"

srcml --archive -l C++ --tabs 4 --position < sub/a.cpp
check 3<<< "$srcml"

srcml --archive -l C++ --position --tabs 4 < sub/a.cpp
check 3<<< "$srcml"

srcml --tabs 4 --position --archive sub/a.cpp
check 3<<< "$fsrcml"

srcml --position --tabs 4 --archive sub/a.cpp
check 3<<< "$fsrcml"

srcml --archive --tabs 4 --position sub/a.cpp
check 3<<< "$fsrcml"

srcml --archive --position --tabs 4 sub/a.cpp
check 3<<< "$fsrcml"

srcml --tabs 4 --position sub/a.cpp --archive
check 3<<< "$fsrcml"

srcml --position --tabs 4 sub/a.cpp --archive
check 3<<< "$fsrcml"

srcml sub/a.cpp --archive --tabs 4 --position
check 3<<< "$fsrcml"

srcml sub/a.cpp --archive --position --tabs 4
check 3<<< "$fsrcml"

srcml -l C++ --tabs 4 -o sub/a.cpp.xml --archive --position < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ -o sub/a.cpp.xml --tabs 4 --archive --position < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -o sub/a.cpp.xml --tabs 4 -l C++ --archive --position < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ --tabs 4 --archive -o sub/a.cpp.xml --position < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml  --archive -l C++ -o sub/a.cpp.xml --tabs 4 --position < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml --tabs 4 --position sub/a.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --position sub/a.cpp --archive -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --position --tabs 4 sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --archive --position sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml sub/a.cpp --position --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml --archive sub/a.cpp --position
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --archive --position -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --position --tabs 4 -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"
