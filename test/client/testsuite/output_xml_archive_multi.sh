#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# xml flag

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define outputa <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define outputb <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define output <<- 'STDOUT'
	STDOUT

#xmlcheck "$output"
xmlcheck "$outputa"
xmlcheck "$outputb"
xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

# file before options
srcml sub/a.cpp.xml -X --unit 1 -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputa"

srcml sub/a.cpp.xml --unit 1 -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputa"

srcml sub/a.cpp.xml -X --unit 1
check "$outputa"

srcml sub/a.cpp.xml --unit 1 -X
check "$outputa"

srcml sub/a.cpp.xml -X --unit 2 -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputb"

srcml sub/a.cpp.xml --unit 2 -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputb"

srcml sub/a.cpp.xml -X --unit 2
check "$outputb"

srcml sub/a.cpp.xml --unit 2 -X
check "$outputb"

srcml sub/a.cpp.xml -X
check "$srcml"

# options before file
srcml -X --unit 1 sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputa"

srcml --unit 1 -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputa"

srcml -X --unit 1 sub/a.cpp.xml
check "$outputa"

srcml --unit 1 -X sub/a.cpp.xml
check "$outputa"

srcml -X --unit 1 -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$outputa"

srcml --unit 1 -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$outputa"

srcml -X --unit 2 sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputb"

srcml --unit 2 -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputb"

srcml -X --unit 2 sub/a.cpp.xml
check "$outputb"

srcml --unit 2 -X sub/a.cpp.xml
check "$outputb"

srcml -X --unit 2 -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$outputb"

srcml --unit 2 -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$outputb"

srcml -X sub/a.cpp.xml
check "$srcml"

srcml -X < sub/a.cpp.xml
check "$srcml"

# XML from standard in
echo "$srcml" | srcml -X --unit 1 -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputa"

echo "$srcml" | srcml --unit 1 -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputa"

echo "$srcml" | srcml -X --unit 1
check "$outputa"

echo "$srcml" | srcml --unit 1 -X
check "$outputa"

echo "$srcml" | srcml -X --unit 2 -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputb"

echo "$srcml" | srcml --unit 2 -X -o sub/b.cpp.xml
check sub/b.cpp.xml "$outputb"

echo "$srcml" | srcml -X --unit 2
check "$outputb"

echo "$srcml" | srcml --unit 2 -X
check "$outputb"

echo "$srcml" | srcml -X
check "$srcml"

