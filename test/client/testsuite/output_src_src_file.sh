#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define src <<- 'STDOUT'
	a;
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

# src --> srcml : input single source file
xmlcheck "$srcml"
createfile sub/a.cpp "$src"

srcml --output-src sub/a.cpp
check "$src"

srcml -S sub/a.cpp
check "$src"

srcml -l C++ -S < sub/a.cpp
check "$src"

srcml -S sub/a.cpp -o sub/b.cpp
check sub/b.cpp "$src"

srcml -S -o sub/b.cpp sub/a.cpp
check sub/b.cpp "$src"

srcml -l C++ -S -o sub/b.cpp < sub/a.cpp
check sub/b.cpp "$src"
