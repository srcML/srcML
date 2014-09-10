#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test output_file
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

src2srcml -l C++ <<< "
a;"

check 3<<< "$output"

mkdir -p sub

define sfile1 <<- 'STDOUT'
	a;
	STDOUT

src2srcml -l C++ - -o sub/a.cpp.xml <<< "
a;"

check sub/a.cpp.xml 3<<< "$output"

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

src2srcml -l C++ <<< "
b;"

check 3<<< "$output"

src2srcml -l C++ - -o sub/b.cpp.xml <<< "
b;"

check sub/b.cpp.xml 3<<< "$output"

