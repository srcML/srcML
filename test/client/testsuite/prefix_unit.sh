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
check ""

srcml sub/a.xml -p "http://www.srcML.org/srcML/src"
check ""

srcml -p "http://www.srcML.org/srcML/cpp" sub/a.xml
check "cpp"

srcml sub/a.xml -p "http://www.srcML.org/srcML/cpp"
check "cpp"

srcml -p "foo.com" sub/a.xml
check "foo"

srcml sub/a.xml -p "foo.com"
check "foo"

srcml -p "nonexistent.com" sub/a.xml
check

srcml sub/a.xml -p "nonexistent.com"
check

# standard input
srcml -p "http://www.srcML.org/srcML/src" <<< "$input"
check ""

srcml --prefix "http://www.srcML.org/srcML/src" <<< "$input"
check ""

srcml --prefix="http://www.srcML.org/srcML/src" <<< "$input"
check ""

srcml -p "http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp"

srcml --prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp"

srcml --prefix="http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp"

srcml --prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp"

srcml -p "http://www.cs.uakron.edu/~collard/foo" <<< "$input"
check

srcml --prefix "http://www.cs.uakron.edu/~collard/foo" <<< "$input"
check

srcml --prefix="http://www.cs.uakron.edu/~collard/foo" <<< "$input"
check

