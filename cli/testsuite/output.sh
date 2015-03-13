#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# Test output options

# src2srcml
define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

createfile sub/a.cpp "a;"


src2srcml sub/a.cpp --output sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml sub/a.cpp --output=sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml -l C++ - -o sub/a.cpp.xml < sub/a.cpp

check sub/a.cpp.xml 3<<< "$xmlfile"

src2srcml -l C++ -o sub/a.cpp.xml sub/a.cpp

check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml -l C++ - --output /dev/stdout < sub/a.cpp

check 3<<< "$xmlfile"

