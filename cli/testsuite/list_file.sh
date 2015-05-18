#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define output <<- 'STDOUT'
	Source encoding: (null)
	XML encoding: UTF-8
	    1 a.cpp	C++	
	Total: 1
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# test --list on input archives
srcml sub/a.cpp.xml --list
check 3<<< "$output"

srcml --list sub/a.cpp.xml
check 3<<< "$output"

srcml --list < sub/a.cpp.xml
check 3<<< "$output"

