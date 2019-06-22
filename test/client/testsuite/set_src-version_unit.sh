#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test src-version_attribute
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" version="1.0"/>
	STDOUT

# file input
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp" version="1.0"/>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$fsrcml"
createfile sub/a.cpp ""

srcml sub/a.cpp -s "1.0"
check "$fsrcml"

srcml sub/a.cpp --src-version "1.0"
check "$fsrcml"

srcml sub/a.cpp --src-version="1.0"
check "$fsrcml"

srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -s '1.0' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

# standard input
srcml -l C++ -s '1.0' -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

echo -n "" | srcml -l C++ -s 1.0
check "$srcml"

echo -n "" | srcml -l C++ --src-version 1.0
check "$srcml"

echo -n "" | srcml -l C++ --src-version=1.0
check "$srcml"

