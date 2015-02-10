#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define src <<- 'STDOUT'
	a;
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define output <<- 'STDOUT'
	1
	STDOUT

# test --show-unit-count on xml files
createfile sub/a.cpp.xml "$srcml"

srcml2src --show-unit-count sub/a.cpp.xml
check 3<<< "$output"

srcml2src --show-unit-count < sub/a.cpp.xml
check 3<<< "$output"

# test --show-unit-count on src files
createfile sub/a.cpp "$src"

src2srcml sub/a.cpp --show-unit-count
check 3<<< "$output"

src2srcml --show-unit-count sub/a.cpp
check 3<<< "$output"

# test --count on empty file
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION"/>
	STDOUT

define empty_output <<- 'STDOUT'
	1
	STDOUT

createfile sub/empty.xml "$empty"

srcml2src --count sub/empty.xml
check 3<<< "$empty_output"

srcml2src --count < sub/empty.xml
check 3<<< "$empty_output"

