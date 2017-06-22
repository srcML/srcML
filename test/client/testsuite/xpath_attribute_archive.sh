#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name foo:foo="test">a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name foo:foo="test">b</name></expr>;</expr_stmt>
	</unit>

	</unit>
  STDOUT

xmlcheck "$result"

createfile a.cpp "a;
"
createfile b.cpp "b;
"

srcml a.cpp b.cpp --xmlns:foo=foo.com -o archive.xml

# from the files
srcml archive.xml --xpath="//src:name" --attribute="foo:foo=test"
check "$result"

srcml --xpath="//src:name" archive.xml --attribute="foo:foo=test"
check "$result"

# output to a file
srcml archive.xml --xpath="//src:name" --attribute="foo:foo=test" -o result.xml
check result.xml "$result"

srcml --xpath="//src:name" archive.xml --attribute="foo:foo=test" -o result.xml
check result.xml "$result"
