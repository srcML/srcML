#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define src <<- 'STDOUT'
	a;
	STDOUT

define output <<- 'STDOUT'
	1
	STDOUT

# test --count on srcml files
createfile sub/a.cpp.xml "$srcml"

srcml2src --count sub/a.cpp.xml
check 3<<< "$output"

srcml2src --count < sub/a.cpp.xml
check 3<<< "$output"

# test --count on src files
createfile sub/a.cpp "$src"

src2srcml --count sub/a.cpp
check 3<<< "$output"

src2srcml sub/a.cpp --count
check 3<<< "$output"

