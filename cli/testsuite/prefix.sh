#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# prefix extraction
define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp"/>
	STDIN

srcml2src -p "http://www.srcML.org/srcML/src" <<< "$input"

check 3<<< ""

srcml2src --prefix "http://www.srcML.org/srcML/src" <<< "$input"

check 3<<< ""

srcml2src --prefix="http://www.srcML.org/srcML/src" <<< "$input"

check 3<<< ""

srcml2src -p "http://www.srcML.org/srcML/cpp" <<< "$input"

check 3<<< "cpp"

srcml2src --prefix "http://www.srcML.org/srcML/cpp" <<< "$input"

check 3<<< "cpp"

srcml2src --prefix="http://www.srcML.org/srcML/cpp" <<< "$input"

check 3<<< "cpp"

srcml2src -p "http://www.cs.uakron.edu/~collard/foo" <<< "$input"

check_null

srcml2src --prefix "http://www.cs.uakron.edu/~collard/foo" <<< "$input"

check_null

srcml2src --prefix="http://www.cs.uakron.edu/~collard/foo" <<< "$input"

check_null

