#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test srcml_extraction

define src <<< "a;"

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

srcml2src <<< "$output"

check 3<<< "$src"

echo "$output" > sub/a.cpp.xml

srcml2src sub/a.cpp.xml

check 3<<< "$src"

