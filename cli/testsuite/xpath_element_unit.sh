#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><foo:foo><name>a</name></foo:foo></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp "a;
"
srcml sub/a.cpp --xmlns:foo=foo.com -o sub/a.xml

# from a file
srcml sub/a.xml --xpath="//src:name" --element="foo:foo"
check 3<<< "$result"

srcml --xpath="//src:name" sub/a.xml --element="foo:foo"
check 3<<< "$result"

# from standard input
echo "a;" | srcml -l C++ --xpath="//src:name" --element="foo:foo"
check 3<<< "$result"

# output to a file
srcml sub/a.xml --xpath="//src:name" --element="foo:foo" -o result.xml
check result.xml <<< "$result"

srcml --xpath="//src:name" sub/a.xml --element="foo:foo" -o result.xml
check result.xml <<< "$result"

echo "a;" | srcml -l C++ --xpath="//src:name" --element="foo:foo" -o result.xml
check result.xml <<< "$result"
