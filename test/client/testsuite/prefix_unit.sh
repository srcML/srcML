#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# prefix extraction
define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:foo="foo.com" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
  STDIN

xmlcheck "$input"
createfile sub/a.xml "$input"

# from file
srcml -p "http://www.srcML.org/srcML/src" sub/a.xml
checkv2 ""

srcml sub/a.xml -p "http://www.srcML.org/srcML/src"
checkv2 ""

srcml -p "http://www.srcML.org/srcML/cpp" sub/a.xml
checkv2 "cpp"

srcml sub/a.xml -p "http://www.srcML.org/srcML/cpp"
checkv2 "cpp"

srcml -p "foo.com" sub/a.xml
checkv2 "foo"

srcml sub/a.xml -p "foo.com"
checkv2 "foo"

srcml -p "nonexistent.com" sub/a.xml
checkv2_null

srcml sub/a.xml -p "nonexistent.com"
checkv2_null

# standard input
srcml2src -p "http://www.srcML.org/srcML/src" <<< "$input"
checkv2 ""

srcml2src --prefix "http://www.srcML.org/srcML/src" <<< "$input"
checkv2 ""

srcml2src --prefix="http://www.srcML.org/srcML/src" <<< "$input"
checkv2 ""

srcml2src -p "http://www.srcML.org/srcML/cpp" <<< "$input"
checkv2 "cpp"

srcml2src --prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
checkv2 "cpp"

srcml2src --prefix="http://www.srcML.org/srcML/cpp" <<< "$input"
checkv2 "cpp"

srcml2src --prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
checkv2 "cpp"

srcml2src -p "http://www.cs.uakron.edu/~collard/foo" <<< "$input"
checkv2_null

srcml2src --prefix "http://www.cs.uakron.edu/~collard/foo" <<< "$input"
checkv2_null

srcml2src --prefix="http://www.cs.uakron.edu/~collard/foo" <<< "$input"
checkv2_null

