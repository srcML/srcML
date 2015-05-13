#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test compression tool

define sfile <<- 'STDOUT'
	a;
	STDOUT

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename=sub/a.cpp>
	</unit>
	STDOUT

# TODO: issue #1057

# src2srcml
createfile sub/a.cpp "$sfile"

src2srcml -z sub/a.cpp -o sub/a.cpp.xml.gz
gunzip -c sub/a.cpp.xml.gz
check 3<<< "$xmlfile"

src2srcml --archive sub/a.cpp -o sub/a.cpp.xml.gz
gunzip -c sub/a.cpp.xml.gz
check 3<<< "$xmlfile"

srcml -l C++ -z -o sub/a.cpp.xml.gz
gunzip -c sub/a.cpp.xml.gz
check 3<<< "$xmlfile"

# srcml2src
createfile sub/a.cpp.xml "$xmlfile"

srcml2src -z sub/a.cpp.xml -o sub/a.cpp.gz
gunzip -c sub/a.cpp.gz
check 3<<< "$sfile"

srcml2src --archive sub/a.cpp.xml -o sub/a.cpp.gz
gunzip -c sub/a.cpp.gz
check 3<<< "$sfile"

srcml2src -z -o sub/a.cpp.gz
gunzip -c sub/a.cpp.gz
check 3<<< "$sfile"

