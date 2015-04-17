#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"><expr_stmt><expr><name cpp:foo="test">a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile a.cpp "a;
"

# from a file
src2srcml a.cpp --xpath="//src:name" --attribute="cpp:foo=test"
check 3<<< "$result"

src2srcml a.cpp --attribute="cpp:foo=test" --xpath="//src:name"
check 3<<< "$result"

src2srcml --attribute="cpp:foo=test" --xpath="//src:name" a.cpp
check 3<<< "$result"

src2srcml --xpath="//src:name" a.cpp --attribute="cpp:foo=test"
check 3<<< "$result"

# from standard input
echo "a;" | src2srcml -l C++ --xpath="//src:name" --attribute="cpp:foo=test"
check 3<<< "$result"

echo "a;" | src2srcml -l C++ --attribute="cpp:foo=test" --xpath="//src:name"
check 3<<< "$result"

# output to a file
src2srcml a.cpp --xpath="//src:name" --attribute="cpp:foo=test" -o result.xml
check result.xml <<< "$result"

src2srcml a.cpp --attribute="cpp:foo=test" --xpath="//src:name" -o result.xml
check result.xml <<< "$result"

src2srcml --attribute="cpp:foo=test" --xpath="//src:name" a.cpp -o result.xml
check result.xml <<< "$result"

src2srcml --xpath="//src:name" a.cpp --attribute="cpp:foo=test" -o result.xml
check result.xml <<< "$result"

echo "a;" | src2srcml -l C++ --xpath="//src:name" --attribute="cpp:foo=test" -o result.xml
check result.xml <<< "$result"

echo "a;" | src2srcml -l C++ --attribute="cpp:foo=test" --xpath="//src:name" -o result.xml
check result.xml <<< "$result"
