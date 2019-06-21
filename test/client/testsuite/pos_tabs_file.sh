#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="4"/>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" pos:tabs="4"/>
	STDOUT

xmlcheck "$fsrcml"
xmlcheck "$srcml"
createfile sub/a.cpp ""

srcml -l C++ --position --tabs 4 < sub/a.cpp
check "$srcml"

srcml -l C++ --tabs 4 --position < sub/a.cpp
check "$srcml"

srcml --position --tabs 4 sub/a.cpp
check "$fsrcml"

srcml --tabs 4 --position sub/a.cpp
check "$fsrcml"

srcml -l C++ --position --tabs 4 -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml -l C++ --tabs 4 --position -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml --tabs 4 --position sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml --position --tabs 4 sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml --tabs 4 --position -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"

srcml --position --tabs 4 -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"

srcml --xmlns:pos=http://www.srcML.org/srcML/position --tabs 4 -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"
