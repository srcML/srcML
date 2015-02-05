#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" language="C++" pos:tabs="8"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
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
