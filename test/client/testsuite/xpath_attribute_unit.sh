#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define resultstdin <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name cpp:foo="test">a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$resultstdin"

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name cpp:foo="test">a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$result"
createfile a.cpp "a;
"

# from a file
srcml a.cpp --xpath="//src:name" --attribute="cpp:foo=test"
check "$result"

srcml --xpath="//src:name" a.cpp --attribute="cpp:foo=test"
check "$result"

# from standard input
echo "a;" | srcml -l C++ --xpath="//src:name" --attribute="cpp:foo=test"
check "$resultstdin"

# output to a file
srcml a.cpp --xpath="//src:name" --attribute="cpp:foo=test" -o result.xml
check result.xml "$result"

srcml --xpath="//src:name" a.cpp --attribute="cpp:foo=test" -o result.xml
check result.xml "$result"

echo "a;" | srcml -l C++ --xpath="//src:name" --attribute="cpp:foo=test" -o result.xml
check result.xml "$resultstdin"
