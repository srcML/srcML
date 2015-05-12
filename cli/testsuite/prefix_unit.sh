#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# prefix extraction
define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:foo="foo.com" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDIN

createfile sub/a.xml "$input"

# from file
srcml -p "http://www.sdml.info/srcML/src" sub/a.xml
check 3<<< ""

srcml sub/a.xml -p "http://www.sdml.info/srcML/src"
check 3<<< ""

srcml -p "http://www.sdml.info/srcML/cpp" sub/a.xml
check 3<<< "cpp"

srcml sub/a.xml -p "http://www.sdml.info/srcML/cpp"
check 3<<< "cpp"

srcml -p "foo.com" sub/a.xml
check 3<<< "foo"

srcml sub/a.xml -p "foo.com"
check 3<<< "foo"

srcml -p "nonexistent.com" sub/a.xml
check_null

srcml sub/a.xml -p "nonexistent.com"
check_null

# standard input
srcml2src -p "http://www.sdml.info/srcML/src" <<< "$input"
check 3<<< ""

srcml2src --prefix "http://www.sdml.info/srcML/src" <<< "$input"
check 3<<< ""

srcml2src --prefix="http://www.sdml.info/srcML/src" <<< "$input"
check 3<<< ""

srcml2src -p "http://www.sdml.info/srcML/cpp" <<< "$input"
check 3<<< "cpp"

srcml2src --prefix "http://www.sdml.info/srcML/cpp" <<< "$input"
check 3<<< "cpp"

srcml2src --prefix="http://www.sdml.info/srcML/cpp" <<< "$input"
check 3<<< "cpp"

srcml2src -p "http://www.cs.uakron.edu/~collard/foo" <<< "$input"
check_null

srcml2src --prefix "http://www.cs.uakron.edu/~collard/foo" <<< "$input"
check_null

srcml2src --prefix="http://www.cs.uakron.edu/~collard/foo" <<< "$input"
check_null

