#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# xml flag

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

# file before options
srcml sub/a.cpp.xml -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml sub/a.cpp.xml -X
check "$srcml"

srcml sub/a.cpp.xml -o sub/b.cpp.xml -X
check sub/b.cpp.xml "$srcml"

# options before file
srcml -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml -X sub/a.cpp.xml
check "$srcml"

srcml -o sub/b.cpp.xml -X < sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml -o sub/b.cpp.xml -X sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

# XML from standard in
echo "$srcml" | srcml -X
check "$srcml"

echo "$srcml" | srcml -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"
