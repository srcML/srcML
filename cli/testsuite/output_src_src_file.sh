#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define src <<- 'STDOUT'
	a;
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

# src2srcml - input single source file
createfile sub/a.cpp "$src"

# TODO: issue #1073
src2srcml --output-src sub/a.cpp
check 3<<< "$src"

src2srcml -S sub/a.cpp
check 3<<< "$src"

src2srcml -l C++ -S < sub/a.cpp
check 3<<< "$src"

src2srcml -S sub/a.cpp -o sub/b.cpp
check sub/b.cpp 3<<< "$src"

src2srcml -S -o sub/b.cpp sub/a.cpp
check sub/b.cpp 3<<< "$src"

src2srcml -l C++ -S -o sub/b.cpp < sub/a.cpp
check sub/b.cpp 3<<< "$src"
