#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# xml flag

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++">
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# file before options
srcml2src sub/a.cpp.xml -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src sub/a.cpp.xml -o sub/b.cpp.xml -X
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src sub/a.cpp.xml -X
check 3<<< "$srcml"

srcml2src sub/a.cpp.xml -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

# options before file
srcml2src -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -o sub/b.cpp.xml -X sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -X sub/a.cpp.xml
check 3<<< "$srcml"

srcml2src -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -o sub/b.cpp.xml -X < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

# XML from standard in
echo "$srcml" | srcml2src -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

echo "$srcml" | srcml2src -o sub/b.cpp.xml -X
check sub/b.cpp.xml 3<<< "$srcml"

echo "$srcml" | srcml2src -X
check 3<<< "$srcml"

echo "$srcml" | srcml2src -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

