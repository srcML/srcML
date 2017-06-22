#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="4"/>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" pos:tabs="4"/>
	STDOUT

xmlcheck "$fsrcml"
xmlcheck "$srcml"
createfile sub/a.cpp ""

srcml -l C++ --position --tabs 4 < sub/a.cpp
checkv2 "$srcml"

srcml -l C++ --tabs 4 --position < sub/a.cpp
checkv2 "$srcml"

srcml --position --tabs 4 sub/a.cpp
checkv2 "$fsrcml"

srcml --tabs 4 --position sub/a.cpp
checkv2 "$fsrcml"

srcml -l C++ --position --tabs 4 -o sub/a.cpp.xml < sub/a.cpp
checkv2 sub/a.cpp.xml "$srcml"

srcml -l C++ --tabs 4 --position -o sub/a.cpp.xml < sub/a.cpp
checkv2 sub/a.cpp.xml "$srcml"

srcml --tabs 4 --position sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --position --tabs 4 sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --tabs 4 --position -o sub/a.cpp.xml sub/a.cpp
checkv2 sub/a.cpp.xml "$fsrcml"

srcml --position --tabs 4 -o sub/a.cpp.xml sub/a.cpp
checkv2 sub/a.cpp.xml "$fsrcml"
