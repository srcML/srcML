#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# testing for verbose
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
  STDOUT

define output <<- 'STDERR'
	Source encoding:  (null)
	XML encoding:  UTF-8
	    1 sub/a.cpp	C++	1	

	Translated: 1	Skipped: 0	Error: 0	Total: 1
  STDERR

xmlcheck "$fsrcml"
createfile sub/a.cpp "a;"
createfile sub/a.cpp.xml "$fsrcml"
 
# from a file
srcml sub/a.cpp --verbose
checkv2 "$fsrcml" 4<<< "$output"

srcml --verbose sub/a.cpp
checkv2 "$fsrcml" 4<<< "$output"

srcml sub/a.cpp --verbose -o sub/c.cpp.xml
checkv2 sub/c.cpp.xml "$fsrcml" 4<<< "$output"

srcml --verbose sub/a.cpp -o sub/c.cpp.xml
checkv2 sub/c.cpp.xml "$fsrcml" 4<<< "$output"

# from standard in
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
  STDOUT

define output <<- 'STDERR'
	Source encoding:  (null)
	XML encoding:  UTF-8
	    1 	C++	1	

	Translated: 1	Skipped: 0	Error: 0	Total: 1
  STDERR

xmlcheck "$srcml"

srcml --verbose -l C++ < sub/a.cpp
checkv2 "$srcml" 4<<< "$output"

srcml --verbose -l C++ -o sub/c.cpp.xml < sub/a.cpp
checkv2 sub/c.cpp.xml "$srcml" 4<<< "$output"

# srcml to src
srcml --verbose sub/a.cpp.xml -o sub/c.cpp
checkv2_null

srcml --verbose -l C++ -o sub/c.cpp < sub/a.cpp.xml
checkv2_null
