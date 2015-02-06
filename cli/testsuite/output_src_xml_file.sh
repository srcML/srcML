#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define src <<- 'STDOUT'
	a;
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

# srcml2src - input srcml single file
createfile sub/a.cpp.xml "$srcml"

srcml2src --output-src sub/a.cpp.xml
check 3<<< "$src"

srcml2src -S sub/a.cpp.xml
check 3<<< "$src"

src2srcml -S < sub/a.cpp.xml
check 3<<< "$src"

src2srcml -S sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp 3<<< "$src"

src2srcml -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp 3<<< "$src"

src2srcml -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp 3<<< "$src"
