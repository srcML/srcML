#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# no xml declaration srcml2src
define srcml <<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT


createfile sub/a.cpp.xml "$srcml"

srcml2src --xml --no-xml-declaration

check sub/a.cpp.xml 3<<< "$srcmlout"

srcml2src --no-xml-declaration <<< "$srcml"

check sub/b.cpp.xml 3<<< "$srcmlout"

