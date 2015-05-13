#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" pos:tabs="8"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
	STDOUT

createfile sub/a.cpp ""

src2srcml -l C++ --position < sub/a.cpp
check 3<<< "$srcml"

src2srcml -l C++ --position -o sub/b.cpp < sub/a.cpp
check sub/b.cpp 3<<< "$srcml"

src2srcml sub/a.cpp --position
check 3<<< "$fsrcml"

src2srcml --position sub/a.cpp
check 3<<< "$fsrcml"
