#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define resultstdin <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION" language="C++"><expr_stmt><expr><pre:element><name>a</name></pre:element></expr>;</expr_stmt>
	</unit>
	STDOUT

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><pre:element><name>a</name></pre:element></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$result"
createfile sub/a.cpp "a;
"
srcml sub/a.cpp --xmlns:pre=foo.com -o sub/a.xml

# from a file
srcml sub/a.xml --xpath="//src:name" --element="pre:element" --xmlns:pre=foo.com
check "$result"

srcml --xpath="//src:name" sub/a.xml --element="pre:element" --xmlns:pre=foo.com
check "$result"

# from standard input
echo "a;" | srcml -l C++ --xmlns:pre=foo.com --xpath="//src:name" --element="pre:element" --xmlns:pre=foo.com
check "$resultstdin"

# output to a file
srcml sub/a.xml --xpath="//src:name" --element="pre:element" --xmlns:pre=foo.com -o result.xml
check result.xml "$result"

srcml --xpath="//src:name" sub/a.xml --element="pre:element" --xmlns:pre=foo.com -o result.xml
check result.xml "$result"

echo "a;" | srcml -l C++  --xmlns:pre=foo.com --xpath="//src:name" --element="pre:element" --xmlns:pre=foo.com -o result.xml
check result.xml "$resultstdin"
