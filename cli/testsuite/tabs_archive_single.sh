#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" pos:tabs="4">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" pos:tabs="4" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" pos:tabs="4">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" pos:tabs="4" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""

# TODO: issue #1051
srcml -l C++ --tabs 4 --archive < sub/a.cpp
check 3<<< "$srcml"

srcml -l C++ --archive --tabs 4 < sub/a.cpp
check 3<<< "$srcml"

srcml --archive -l C++ --tabs 4 < sub/a.cpp
check 3<<< "$srcml"

srcml --tabs 4 --archive sub/a.cpp
check 3<<< "$fsrcml"

srcml --archive --tabs 4 sub/a.cpp
check 3<<< "$fsrcml"

srcml --tabs 4 sub/a.cpp --archive
check 3<<< "$fsrcml"

srcml sub/a.cpp --archive --tabs 4
check 3<<< "$fsrcml"

srcml -l C++ --tabs 4 -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ -o sub/a.cpp.xml --tabs 4 --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -o sub/a.cpp.xml --tabs 4 -l C++ --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml -l C++ --tabs 4 --archive -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml  --archive -l C++ -o sub/a.cpp.xml --tabs 4 < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml --tabs 4 sub/a.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 sub/a.cpp --archive -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 4 sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --archive sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml sub/a.cpp --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml --archive sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --archive -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 4 -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"
