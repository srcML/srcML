#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# filename flag
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="foo.cpp"/>
	STDOUT

createfile sub/a.cpp ""

src2srcml sub/a.cpp -f "foo.cpp"

check 3<<< "$output"

src2srcml sub/a.cpp --filename "foo.cpp"

check 3<<< "$output"

src2srcml sub/a.cpp --filename="foo.cpp"

check 3<<< "$output"

src2srcml -l C++ -f 'foo.cpp' -o sub/a.cpp.xml sub/a.cpp

check sub/a.cpp.xml 3<<< "$output"

src2srcml -f 'foo.cpp' sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$output"
