#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# xml flag

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT


createfile sub/a.cpp.xml "$srcml"

# file before options
srcml2src sub/a.cpp.xml -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src sub/a.cpp.xml -X
check 3<<< "$srcml"

srcml2src sub/a.cpp.xml -o sub/b.cpp.xml -X
check sub/b.cpp.xml 3<<< "$srcml"

# options before file
srcml2src -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -X sub/a.cpp.xml
check 3<<< "$srcml"

srcml2src -o sub/b.cpp.xml -X < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -o sub/b.cpp.xml -X sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

# XML from standard in
echo "$srcml" | srcml -X
check 3<<< "$srcml"

echo "$srcml" | srcml -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"
