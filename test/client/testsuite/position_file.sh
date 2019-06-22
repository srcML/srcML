#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" pos:tabs="8"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$fsrcml"
createfile sub/a.cpp ""

srcml -l C++ --position < sub/a.cpp
check "$srcml"

srcml -l C++ --position -o sub/b.cpp < sub/a.cpp
check sub/b.cpp "$srcml"

srcml sub/a.cpp --position
check "$fsrcml"

srcml --position sub/a.cpp
check "$fsrcml"
