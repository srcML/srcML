#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test input file is gzipped

define sfile <<< "a;"

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define fxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp.gz"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp "a;\n"

rmfile sub/a.cpp.gz

gzip sub/a.cpp

src2srcml sub/a.cpp.gz

check 3<<< "$fxmlfile"

cat sub/a.cpp.gz | src2srcml -l C++ -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml sub/a.cpp.gz -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$fxmlfile"

src2srcml sub/a.cpp.gz -o sub/a.cpp.xml.gz
	
cat sub/a.cpp.xml.gz | srcml2src

check 3<<< "a;"

srcml2src sub/a.cpp.xml.gz

check 3<<< "a;"

cat sub/a.cpp.xml.gz | srcml2src -o sub/a.cpp

check sub/a.cpp 3<<< "a;"

srcml2src sub/a.cpp.xml.gz -o sub/a.cpp

check sub/a.cpp 3<<< "a;"
