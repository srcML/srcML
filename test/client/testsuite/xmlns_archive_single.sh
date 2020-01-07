#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# default xmlns
define foosrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<bar:unit xmlns:bar="http://www.srcML.org/srcML/src" xmlns="http://www.foo.com" revision="REVISION">

	<bar:unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><bar:expr_stmt><bar:expr><bar:name>a</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	</bar:unit>
	STDOUT

xmlcheck "$foosrcml"
createfile sub/a.cpp "a;"

srcml --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp --archive
check "$foosrcml"

srcml sub/a.cpp --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src --archive
check "$foosrcml"

srcml sub/a.cpp --archive --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src
check "$foosrcml"

srcml --archive --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp
check "$foosrcml"

srcml --archive sub/a.cpp --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src
check "$foosrcml"

srcml --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp --archive -o sub/a.xml
check sub/a.xml "$foosrcml"

srcml --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp -o sub/a.xml --archive
check sub/a.xml "$foosrcml"

srcml --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src -o sub/a.xml sub/a.cpp --archive
check sub/a.xml "$foosrcml"

srcml -o sub/a.xml --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp --archive
check sub/a.xml "$foosrcml"

srcml sub/a.cpp --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src --archive -o sub/a.xml
check sub/a.xml "$foosrcml"

srcml sub/a.cpp --archive -o sub/a.xml --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml "$foosrcml"

srcml sub/a.cpp -o sub/a.xml --archive --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml "$foosrcml"

srcml -o sub/a.xml sub/a.cpp --archive --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml "$foosrcml"

srcml --archive --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp -o sub/a.xml
check sub/a.xml "$foosrcml"

srcml --archive --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src -o sub/a.xml sub/a.cpp
check sub/a.xml "$foosrcml"

srcml --archive -o sub/a.xml --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp
check sub/a.xml "$foosrcml"

srcml -o sub/a.xml --archive --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src sub/a.cpp
check sub/a.xml "$foosrcml"

srcml --archive sub/a.cpp --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src -o sub/a.xml
check sub/a.xml "$foosrcml"

srcml --archive sub/a.cpp -o sub/a.xml --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml "$foosrcml"

srcml --archive -o sub/a.xml sub/a.cpp --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml "$foosrcml"

srcml -o sub/a.xml --archive sub/a.cpp --xmlns="http://www.foo.com" --xmlns:bar=http://www.srcML.org/srcML/src
check sub/a.xml "$foosrcml"

# with prefix
define fooprefixsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="http://www.foo.com" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$fooprefixsrcml"

srcml --xmlns:foo="http://www.foo.com" sub/a.cpp --archive
check "$fooprefixsrcml"

srcml sub/a.cpp --xmlns:foo="http://www.foo.com" --archive
check "$fooprefixsrcml"

srcml sub/a.cpp --archive --xmlns:foo="http://www.foo.com"
check "$fooprefixsrcml"

srcml --archive --xmlns:foo="http://www.foo.com" sub/a.cpp
check "$fooprefixsrcml"

srcml --archive sub/a.cpp --xmlns:foo="http://www.foo.com"
check "$fooprefixsrcml"

srcml --xmlns:foo="http://www.foo.com" sub/a.cpp --archive -o sub/a.xml
check sub/a.xml "$fooprefixsrcml"

srcml --xmlns:foo="http://www.foo.com" sub/a.cpp -o sub/a.xml --archive
check sub/a.xml "$fooprefixsrcml"

srcml --xmlns:foo="http://www.foo.com" -o sub/a.xml sub/a.cpp --archive
check sub/a.xml "$fooprefixsrcml"

srcml -o sub/a.xml --xmlns:foo="http://www.foo.com" sub/a.cpp --archive
check sub/a.xml "$fooprefixsrcml"

srcml sub/a.cpp --xmlns:foo="http://www.foo.com" --archive -o sub/a.xml
check sub/a.xml "$fooprefixsrcml"

srcml sub/a.cpp --archive -o sub/a.xml --xmlns:foo="http://www.foo.com"
check sub/a.xml "$fooprefixsrcml"

srcml sub/a.cpp -o sub/a.xml --archive --xmlns:foo="http://www.foo.com"
check sub/a.xml "$fooprefixsrcml"

srcml -o sub/a.xml sub/a.cpp --archive --xmlns:foo="http://www.foo.com"
check sub/a.xml "$fooprefixsrcml"

srcml --archive --xmlns:foo="http://www.foo.com" sub/a.cpp -o sub/a.xml
check sub/a.xml "$fooprefixsrcml"

srcml --archive --xmlns:foo="http://www.foo.com" -o sub/a.xml sub/a.cpp
check sub/a.xml "$fooprefixsrcml"

srcml --archive -o sub/a.xml --xmlns:foo="http://www.foo.com" sub/a.cpp
check sub/a.xml "$fooprefixsrcml"

srcml -o sub/a.xml --archive --xmlns:foo="http://www.foo.com" sub/a.cpp
check sub/a.xml "$fooprefixsrcml"

srcml --archive sub/a.cpp --xmlns:foo="http://www.foo.com" -o sub/a.xml
check sub/a.xml "$fooprefixsrcml"

srcml --archive sub/a.cpp -o sub/a.xml --xmlns:foo="http://www.foo.com"
check sub/a.xml "$fooprefixsrcml"

srcml --archive -o sub/a.xml sub/a.cpp --xmlns:foo="http://www.foo.com"
check sub/a.xml "$fooprefixsrcml"

srcml -o sub/a.xml --archive sub/a.cpp --xmlns:foo="http://www.foo.com"
check sub/a.xml "$fooprefixsrcml"
