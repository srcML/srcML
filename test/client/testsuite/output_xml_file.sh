#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# xml flag

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++">
	</unit>
	STDOUT

xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

# no options
srcml sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

# file before options
srcml sub/a.cpp.xml -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml sub/a.cpp.xml -o sub/b.cpp.xml -X
check sub/b.cpp.xml "$srcml"

srcml sub/a.cpp.xml -X
check "$srcml"

srcml sub/a.cpp.xml -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

# options before file
srcml -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml -o sub/b.cpp.xml -X sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml -X sub/a.cpp.xml
check "$srcml"

srcml -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml -o sub/b.cpp.xml -X < sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

# XML from standard in
echo "$srcml" | srcml -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

echo "$srcml" | srcml -o sub/b.cpp.xml -X
check sub/b.cpp.xml "$srcml"

echo "$srcml" | srcml -X
check "$srcml"

echo "$srcml" | srcml -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

