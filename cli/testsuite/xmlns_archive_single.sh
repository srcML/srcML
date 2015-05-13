#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# default xmlns
define foosrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<bar:unit xmlns:bar="http://www.srcML.org/srcML/src" xmlns="foo.com" revision="REVISION">

	<bar:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><bar:expr_stmt><bar:expr><bar:name>a</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	</bar:unit>
	STDOUT

createfile sub/a.cpp "a;"

srcml --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp --archive
check 3<<< "$foosrcml"

srcml sub/a.cpp --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src --archive
check 3<<< "$foosrcml"

srcml sub/a.cpp --archive --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src
check 3<<< "$foosrcml"

srcml --archive --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp
check 3<<< "$foosrcml"

srcml --archive sub/a.cpp --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src
check 3<<< "$foosrcml"

srcml --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp --archive -o sub/a.xml
check sub/a.xml 3<<< "$foosrcml"

srcml --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp -o sub/a.xml --archive
check sub/a.xml 3<<< "$foosrcml"

srcml --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src -o sub/a.xml sub/a.cpp --archive
check sub/a.xml 3<<< "$foosrcml"

srcml -o sub/a.xml --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp --archive
check sub/a.xml 3<<< "$foosrcml"

srcml sub/a.cpp --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src --archive -o sub/a.xml
check sub/a.xml 3<<< "$foosrcml"

srcml sub/a.cpp --archive -o sub/a.xml --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml 3<<< "$foosrcml"

srcml sub/a.cpp -o sub/a.xml --archive --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml 3<<< "$foosrcml"

srcml -o sub/a.xml sub/a.cpp --archive --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml 3<<< "$foosrcml"

srcml --archive --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp -o sub/a.xml
check sub/a.xml 3<<< "$foosrcml"

srcml --archive --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src -o sub/a.xml sub/a.cpp
check sub/a.xml 3<<< "$foosrcml"

srcml --archive -o sub/a.xml --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp
check sub/a.xml 3<<< "$foosrcml"

srcml -o sub/a.xml --archive --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp
check sub/a.xml 3<<< "$foosrcml"

srcml --archive sub/a.cpp --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src -o sub/a.xml
check sub/a.xml 3<<< "$foosrcml"

srcml --archive sub/a.cpp -o sub/a.xml --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml 3<<< "$foosrcml"

srcml --archive -o sub/a.xml sub/a.cpp --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml 3<<< "$foosrcml"

srcml -o sub/a.xml --archive sub/a.cpp --xmlns=foo.com --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml 3<<< "$foosrcml"


# with prefix
define fooprefixsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT


srcml --xmlns:foo=foo.com sub/a.cpp --archive
check 3<<< "$fooprefixsrcml"

srcml sub/a.cpp --xmlns:foo=foo.com --archive
check 3<<< "$fooprefixsrcml"

srcml sub/a.cpp --archive --xmlns:foo=foo.com
check 3<<< "$fooprefixsrcml"

srcml --archive --xmlns:foo=foo.com sub/a.cpp
check 3<<< "$fooprefixsrcml"

srcml --archive sub/a.cpp --xmlns:foo=foo.com
check 3<<< "$fooprefixsrcml"

srcml --xmlns:foo=foo.com sub/a.cpp --archive -o sub/a.xml
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --xmlns:foo=foo.com sub/a.cpp -o sub/a.xml --archive
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --xmlns:foo=foo.com -o sub/a.xml sub/a.cpp --archive
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml -o sub/a.xml --xmlns:foo=foo.com sub/a.cpp --archive
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml sub/a.cpp --xmlns:foo=foo.com --archive -o sub/a.xml
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml sub/a.cpp --archive -o sub/a.xml --xmlns:foo=foo.com
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml sub/a.cpp -o sub/a.xml --archive --xmlns:foo=foo.com
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml -o sub/a.xml sub/a.cpp --archive --xmlns:foo=foo.com
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --archive --xmlns:foo=foo.com sub/a.cpp -o sub/a.xml
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --archive --xmlns:foo=foo.com -o sub/a.xml sub/a.cpp
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --archive -o sub/a.xml --xmlns:foo=foo.com sub/a.cpp
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml -o sub/a.xml --archive --xmlns:foo=foo.com sub/a.cpp
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --archive sub/a.cpp --xmlns:foo=foo.com -o sub/a.xml
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --archive sub/a.cpp -o sub/a.xml --xmlns:foo=foo.com
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --archive -o sub/a.xml sub/a.cpp --xmlns:foo=foo.com
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml -o sub/a.xml --archive sub/a.cpp --xmlns:foo=foo.com
check sub/a.xml 3<<< "$fooprefixsrcml"


