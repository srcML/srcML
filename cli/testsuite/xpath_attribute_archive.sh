#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name foo:foo="test">a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name foo:foo="test">b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile a.cpp "a;
"
createfile b.cpp "b;
"

srcml a.cpp b.cpp --xmlns:foo=foo.com -o archive.xml

# from the files
srcml archive.xml --xpath="//src:name" --attribute="foo:foo=test"
check 3<<< "$result"

srcml --xpath="//src:name" archive.xml --attribute="foo:foo=test"
check 3<<< "$result"

# output to a file
srcml archive.xml --xpath="//src:name" --attribute="foo:foo=test" -o result.xml
check result.xml 3<<< "$result"

srcml --xpath="//src:name" archive.xml --attribute="foo:foo=test" -o result.xml
check result.xml 3<<< "$result"
