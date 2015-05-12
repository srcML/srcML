#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="foo.com" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"><expr_stmt><expr><foo:foo><name>a</name></foo:foo></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile a.cpp "a;
"

src2srcml a.cpp --xmlns:foo=foo.com -o a.xml

# from a file
src2srcml a.xml --xpath="//src:name" --element="foo:foo"
check 3<<< "$result"

src2srcml a.xml --element="foo:foo" --xpath="//src:name"
check 3<<< "$result"

src2srcml --element="foo:foo" --xpath="//src:name" a.xml
check 3<<< "$result"

src2srcml --xpath="//src:name" a.xml --element="foo:foo"
check 3<<< "$result"

# from standard input
echo "a;" | src2srcml -l C++ --xpath="//src:name" --element="foo:foo"
check 3<<< "$result"

echo "a;" | src2srcml -l C++ --element="foo:foo" --xpath="//src:name"
check 3<<< "$result"

# output to a file
src2srcml a.xml --xpath="//src:name" --element="foo:foo" -o result.xml
check result.xml <<< "$result"

src2srcml a.xml --element="foo:foo" --xpath="//src:name" -o result.xml
check result.xml <<< "$result"

src2srcml --element="foo:foo" --xpath="//src:name" a.xml -o result.xml
check result.xml <<< "$result"

src2srcml --xpath="//src:name" a.xml --element="foo:foo" -o result.xml
check result.xml <<< "$result"

echo "a;" | src2srcml -l C++ --xpath="//src:name" --element="foo:foo" -o result.xml
check result.xml <<< "$result"

echo "a;" | src2srcml -l C++ --element="foo:foo" --xpath="//src:name" -o result.xml
check result.xml <<< "$result"
