#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test srcml_extraction

define src <<< "
a;"

define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDIN

srcml2src <<< "$input"

check 3<<< "$src"

createfile sub/a.cpp.xml "$input"

srcml2src sub/a.cpp.xml

check 3<<< "$src"
