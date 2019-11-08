#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on single unit
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

# test on empty unit
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" language="C++" revision="REVISION"/>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$empty"

# test on src that creates single unit
define src <<- 'STDOUT'
	a;
	STDOUT

define output <<- 'STDOUT'
	1
	STDOUT

createfile sub/a.cpp.xml "$srcml"
createfile sub/a.cpp "$src"
createfile sub/empty.xml "$empty"

srcml --show-unit-count sub/a.cpp.xml
check "$output"

srcml --show-unit-count < sub/a.cpp.xml
check "$output"

srcml sub/a.cpp --show-unit-count
check "$output"

srcml --show-unit-count sub/a.cpp
check "$output"

srcml --show-unit-count -l C++ < sub/a.cpp
check "$output"

srcml -l C++ --show-unit-count < sub/a.cpp
check "$output"

srcml --show-unit-count  sub/empty.xml
check "$output"

srcml --show-unit-count  < sub/empty.xml
check "$output"

