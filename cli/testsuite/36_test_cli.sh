#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# empty with debug
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

src2srcml sub/a.cpp -g
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --debug
check 3<<< "$fsrcml"

src2srcml -l C++ -g -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -g sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"
