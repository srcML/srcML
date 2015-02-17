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

# test --show-unit-count on srcml files
createfile sub/a.cpp.xml "$srcml"

srcml2src --show-unit-count sub/a.cpp.xml
check 3<<< "$output"

srcml2src --show-unit-count < sub/a.cpp.xml
check 3<<< "$output"

# test --show-unit-count on src files
createfile sub/a.cpp "$src"

src2srcml --show-unit-count sub/a.cpp
check 3<<< "$output"

src2srcml sub/a.cpp --show-unit-count
check 3<<< "$output"

# test --count on empty archive
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

define empty_output <<- 'STDOUT'
	0
	STDOUT

createfile sub/emptyarchive.xml "$empty"

srcml2src --count sub/emptyarchive.xml
check 3<<< "$empty_output"

srcml2src --count < sub/emptyarchive.xml
check 3<<< "$empty_output"
