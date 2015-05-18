#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh
set +e

# must specify attribute after xpath query
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
srcml archive.xml --attribute="foo:foo=test" --xpath="//src:name"
check_exit 2

srcml --attribute="foo:foo=test" --xpath="//src:name" archive.xml
check_exit 2

# output to a file
srcml archive.xml --attribute="foo:foo=test" --xpath="//src:name" -o result.xml
check_exit 2

srcml --attribute="foo:foo=test" --xpath="//src:name" archive.xml -o result.xml
check_exit 2



# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name cpp:foo="test">a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile a.cpp "a;
"

# from a file
srcml a.cpp --attribute="cpp:foo=test" --xpath="//src:name"
check_exit 2

srcml --attribute="cpp:foo=test" --xpath="//src:name" a.cpp
check_exit 2

# from standard input
echo "a;" | srcml -l C++ --attribute="cpp:foo=test" --xpath="//src:name"
check_exit 2

# output to a file
srcml a.cpp --attribute="cpp:foo=test" --xpath="//src:name" -o result.xml
check_exit 2

srcml --attribute="cpp:foo=test" --xpath="//src:name" a.cpp -o result.xml
check_exit 2

echo "a;" | srcml -l C++ --attribute="cpp:foo=test" --xpath="//src:name" -o result.xml
check_exit 2



