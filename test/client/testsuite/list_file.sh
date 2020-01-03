#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define output <<- 'STDOUT'
	XML encoding: UTF-8
	    1  C++     1  a.cpp
	units: 1
	LOC: 1
	STDOUT

xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

# test --list on input archives
srcml sub/a.cpp.xml --list
check "$output"

srcml sub/a.cpp.xml -L
check "$output"

srcml --list sub/a.cpp.xml
check "$output"

srcml -L sub/a.cpp.xml
check "$output"

srcml --list < sub/a.cpp.xml
check "$output"

srcml -L < sub/a.cpp.xml
check "$output"
