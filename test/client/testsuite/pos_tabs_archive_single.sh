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

xmlcheck "$fsrcml"
xmlcheck "$srcml"
createfile sub/a.cpp ""

srcml -l C++ --tabs 4 --position --archive < sub/a.cpp
checkv2 "$srcml"

srcml -l C++ --position --tabs 4 --archive < sub/a.cpp
checkv2 "$srcml"

srcml -l C++ --archive  --position --tabs 4 < sub/a.cpp
checkv2 "$srcml"

srcml -l C++ --archive --tabs 4 --position < sub/a.cpp
checkv2 "$srcml"

srcml --archive -l C++ --tabs 4 --position < sub/a.cpp
checkv2 "$srcml"

srcml --archive -l C++ --position --tabs 4 < sub/a.cpp
checkv2 "$srcml"

srcml --tabs 4 --position --archive sub/a.cpp
checkv2 "$fsrcml"

srcml --position --tabs 4 --archive sub/a.cpp
checkv2 "$fsrcml"

srcml --archive --tabs 4 --position sub/a.cpp
checkv2 "$fsrcml"

srcml --archive --position --tabs 4 sub/a.cpp
checkv2 "$fsrcml"

srcml --tabs 4 --position sub/a.cpp --archive
checkv2 "$fsrcml"

srcml --position --tabs 4 sub/a.cpp --archive
checkv2 "$fsrcml"

srcml sub/a.cpp --archive --tabs 4 --position
checkv2 "$fsrcml"

srcml sub/a.cpp --archive --position --tabs 4
checkv2 "$fsrcml"

srcml -l C++ --tabs 4 -o sub/a.cpp.xml --archive --position < sub/a.cpp
checkv2 sub/a.cpp.xml "$srcml"

srcml -l C++ -o sub/a.cpp.xml --tabs 4 --archive --position < sub/a.cpp
checkv2 sub/a.cpp.xml "$srcml"

srcml -o sub/a.cpp.xml --tabs 4 -l C++ --archive --position < sub/a.cpp
checkv2 sub/a.cpp.xml "$srcml"

srcml -l C++ --tabs 4 --archive -o sub/a.cpp.xml --position < sub/a.cpp
checkv2 sub/a.cpp.xml "$srcml"

srcml  --archive -l C++ -o sub/a.cpp.xml --tabs 4 --position < sub/a.cpp
checkv2 sub/a.cpp.xml "$srcml"

srcml --tabs 4 --position sub/a.cpp -o sub/a.cpp.xml --archive
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --tabs 4 --position sub/a.cpp --archive -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --archive --position --tabs 4 sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --tabs 4 --archive --position sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml sub/a.cpp --position --archive
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml --archive sub/a.cpp --position
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --tabs 4 --archive --position -o sub/a.cpp.xml sub/a.cpp
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --archive --position --tabs 4 -o sub/a.cpp.xml sub/a.cpp
checkv2 sub/a.cpp.xml "$fsrcml"
