#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" pos:tabs="8"/>
	STDOUT

createfile sub/a.cpp ""

# specify tab size of 8
srcml -l C++ --tabs 8 < sub/a.cpp
check 3<<< "$srcml"

srcml --tabs 8 sub/a.cpp
check 3<<< "$fsrcml"

srcml -l C++ --tabs 8 -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml --tabs 8 sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

# use default tab size of 8
srcml -l C++ --tabs < sub/a.cpp
check 3<<< "$srcml"

srcml -l C++ --tabs -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

srcml --tabs -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"
