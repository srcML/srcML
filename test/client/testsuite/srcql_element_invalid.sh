#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test specify element after xpath (archive)
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$result"
createfile a.cpp "a;
"
createfile b.cpp "b;
"

srcml a.cpp b.cpp --xmlns:pre=foo.com -o archive.xml

srcml archive.xml --element="pre:element" --xpath="//src:name"
check_exit 1

srcml --element="pre:element" --xpath="//src:name" archive.xml
check_exit 1

# output to a file
srcml archive.xml --element="pre:element" --xpath="//src:name" -o result.xml
check_exit 1

srcml --element="pre:element" --xpath="//src:name" archive.xml -o result.xml
check_exit 1

# test specify element after xpath (unit)
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION">

	<unit revision="REVISION" language="C++"><expr_stmt><expr><pre:element><name>a</name></pre:element></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$result"
createfile sub/a.cpp "a;
"
srcml sub/a.cpp --xmlns:pre=foo.com -o sub/a.xml

# from a file
srcml sub/a.xml --element="pre:element" --xpath="//src:name"
check_exit 1

srcml --element="pre:element" --xpath="//src:name" sub/a.xml
check_exit 1

# from standard input
echo "a;" | srcml -l C++ --element="pre:element" --xpath="//src:name"
check_exit 1

# output to a file
srcml sub/a.xml --element="pre:element" --xpath="//src:name" -o result.xml
check_exit 1

srcml --element="pre:element" --xpath="//src:name" sub/a.xml -o result.xml
check_exit 1

echo "a;" | srcml -l C++ --element="pre:element" --xpath="//src:name" -o result.xml
check_exit 1
