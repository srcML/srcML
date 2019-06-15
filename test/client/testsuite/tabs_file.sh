#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" pos:tabs="8"/>
	STDOUT

xmlcheck "$fsrcml"
xmlcheck "$srcml"
createfile sub/a.cpp ""

# specify tab size of 8
srcml -l C++ --tabs 8 < sub/a.cpp
check "$srcml"

srcml --tabs 8 sub/a.cpp
check "$fsrcml"

srcml -l C++ --tabs 8 -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml --tabs 8 sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml --tabs 8 -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"
