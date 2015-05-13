#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# xml flag

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define outputa <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define outputb <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define output <<- 'STDOUT'
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# file before options
srcml2src sub/a.cpp.xml -X --unit 1 -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputa"

srcml2src sub/a.cpp.xml --unit 1 -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputa"

srcml2src sub/a.cpp.xml -X --unit 1
check 3<<< "$outputa"

srcml2src sub/a.cpp.xml --unit 1 -X
check 3<<< "$outputa"

srcml2src sub/a.cpp.xml -X --unit 2 -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputb"

srcml2src sub/a.cpp.xml --unit 2 -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputb"

srcml2src sub/a.cpp.xml -X --unit 2
check 3<<< "$outputb"

srcml2src sub/a.cpp.xml --unit 2 -X
check 3<<< "$outputb"

srcml sub/a.cpp.xml -X
check 3<<< "$srcml"

# options before file
srcml2src -X --unit 1 sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputa"

srcml2src --unit 1 -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputa"

srcml2src -X --unit 1 sub/a.cpp.xml
check 3<<< "$outputa"

srcml2src --unit 1 -X sub/a.cpp.xml
check 3<<< "$outputa"

srcml2src -X --unit 1 -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$outputa"

srcml2src --unit 1 -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$outputa"

srcml2src -X --unit 2 sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputb"

srcml2src --unit 2 -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputb"

srcml2src -X --unit 2 sub/a.cpp.xml
check 3<<< "$outputb"

srcml2src --unit 2 -X sub/a.cpp.xml
check 3<<< "$outputb"

srcml2src -X --unit 2 -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$outputb"

srcml2src --unit 2 -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$outputb"

srcml -X sub/a.cpp.xml
check 3<<< "$srcml"

srcml -X < sub/a.cpp.xml
check 3<<< "$srcml"

# XML from standard in
echo "$srcml" | srcml2src -X --unit 1 -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputa"

echo "$srcml" | srcml2src --unit 1 -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputa"

echo "$srcml" | srcml2src -X --unit 1
check 3<<< "$outputa"

echo "$srcml" | srcml2src --unit 1 -X
check 3<<< "$outputa"

echo "$srcml" | srcml2src -X --unit 2 -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputb"

echo "$srcml" | srcml2src --unit 2 -X -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$outputb"

echo "$srcml" | srcml2src -X --unit 2
check 3<<< "$outputb"

echo "$srcml" | srcml2src --unit 2 -X
check 3<<< "$outputb"

echo "$srcml" | srcml -X
check 3<<< "$srcml"

