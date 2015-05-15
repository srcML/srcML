#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# srcML info and longinfo

define info <<- 'STDOUT'
	xmlns=http://www.srcML.org/srcML/src xmlns:cpp=http://www.srcML.org/srcML/cpp
	encoding="UTF-8"
	language="C++"
	url="test"
	filename="sub/a.cpp"
	STDOUT

define longinfo <<- 'STDOUT'
	xmlns=http://www.srcML.org/srcML/src xmlns:cpp=http://www.srcML.org/srcML/cpp
	encoding="UTF-8"
	language="C++"
	url="test"
	filename="sub/a.cpp"
	1
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="test" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

srcml sub/a.cpp.xml -i
check 3<<< "$info"

srcml sub/a.cpp.xml --info
check 3<<< "$info"

srcml --info < sub/a.cpp.xml
check 3<<< "$info"

srcml -i sub/a.cpp.xml
check 3<<< "$info"

srcml --info sub/a.cpp.xml
check 3<<< "$info"

srcml --longinfo sub/a.cpp.xml
check 3<<< "$longinfo"

srcml sub/a.cpp.xml --longinfo
check 3<<< "$longinfo"

srcml --longinfo < sub/a.cpp.xml
check 3<<< "$longinfo"
