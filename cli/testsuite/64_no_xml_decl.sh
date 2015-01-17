#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# no xml declaration srcml2src
define srcml <<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT

define srcmlout <<- STDOUT
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

srcml2src -X --no-xml-declaration < sub/a.cpp.xml

check 3<<< "$srcmlout"

srcml2src -X --no-xml-declaration -o sub/b.cpp.xml < sub/a.cpp.xml

check sub/b.cpp.xml 3<<< "$srcmlout"
rmfile sub/b.cpp.xml

srcml2src -X --no-xml-declaration < sub/a.cpp.xml

check 3<<< "$srcmlout"

