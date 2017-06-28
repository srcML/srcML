#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test compression tool

define sfile <<- 'STDOUT'
	
	a;
	STDOUT

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="0.9.5">
	
	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="0.9.5" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$sxmlfile"
xmlcheck "$xmlfile"

# src2srcml
createfile sub/a.cpp "$sfile"

src2srcml sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$xmlfile"

src2srcml sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$xmlfile"

srcml -l C++ sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$xmlfile"

srcml -l C++ sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$xmlfile"

src2srcml --archive sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$sxmlfile"

src2srcml --archive sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$sxmlfile"

# TODO: issue #1057 - cannot gz a raw source output file
# srcml2src
# createfile sub/a.cpp.xml "$xmlfile"

#srcml2src sub/a.cpp.xml -o sub/a.cpp.gz && gunzip -c sub/a.cpp.gz
#check "$sfile"

#srcml2src --archive sub/a.cpp.xml -o sub/a.cpp.gz && gunzip -c sub/a.cpp.gz
#check "$sfile"

#srcml2src -o sub/a.cpp.gz  && gunzip -c sub/a.cpp.gz
#check "$sfile"

