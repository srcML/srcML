#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# filename flag
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="foo.cpp"/>
	STDOUT

xmlcheck "$output"
createfile sub/a.cpp ""

srcml sub/a.cpp -f "foo.cpp"
check "$output"

srcml sub/a.cpp --filename "foo.cpp"
check "$output"

srcml sub/a.cpp --filename="foo.cpp"
check "$output"

srcml -l C++ -f 'foo.cpp' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$output"

srcml -f 'foo.cpp' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

# standard input
echo -n "" | srcml -l C++ -f foo.cpp
check "$output"

echo -n "" | srcml -l C++ --filename foo.cpp
check "$output"

echo -n "" | srcml -l C++ --filename=foo.cpp
check "$output"
