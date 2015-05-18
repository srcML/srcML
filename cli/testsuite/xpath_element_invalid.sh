#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh
set +e

# test specify element after xpath (archive)
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile a.cpp "a;
"
createfile b.cpp "b;
"
srcml a.cpp b.cpp --xmlns:foo=foo.com -o archive.xml


srcml archive.xml --element="foo:foo" --xpath="//src:name"
check_exit 2

srcml --element="foo:foo" --xpath="//src:name" archive.xml
check_exit 2

# output to a file
srcml archive.xml --element="foo:foo" --xpath="//src:name" -o result.xml
check_exit 2

srcml --element="foo:foo" --xpath="//src:name" archive.xml -o result.xml
check_exit 2



# test specify element after xpath (unit)
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><foo:foo><name>a</name></foo:foo></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp "a;
"
srcml sub/a.cpp --xmlns:foo=foo.com -o sub/a.xml

# from a file
srcml sub/a.xml --element="foo:foo" --xpath="//src:name"
check_exit 2

srcml --element="foo:foo" --xpath="//src:name" sub/a.xml
check_exit 2

# from standard input
echo "a;" | srcml -l C++ --element="foo:foo" --xpath="//src:name"
check_exit 2

# output to a file
srcml sub/a.xml --element="foo:foo" --xpath="//src:name" -o result.xml
check_exit 2

srcml --element="foo:foo" --xpath="//src:name" sub/a.xml -o result.xml
check_exit 2

echo "a;" | srcml -l C++ --element="foo:foo" --xpath="//src:name" -o result.xml
check_exit 2
