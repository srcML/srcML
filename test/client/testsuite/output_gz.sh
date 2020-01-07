#!/bin/bash

libversion=$(srcml --version | tail -1)
libversion=${libversion:10}
numversion=$(printf "%d%03d%03d" ${libversion:1:1} ${libversion:3:1} ${libversion:5:1})
if [[ $numversion -lt 3002000 ]]; then
	echo "Test Skipped: Output as gz not available on this platform"
	exit 0
fi

# test framework
source $(dirname "$0")/framework_test.sh

# test compression tool

define sfile <<- 'STDOUT'

	a;
	STDOUT

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$sxmlfile"
xmlcheck "$xmlfile"

# src --> srcml
createfile sub/a.cpp "$sfile"

srcml sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$xmlfile"

srcml sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$xmlfile"

srcml -l C++ sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$xmlfile"

srcml -l C++ sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$xmlfile"

srcml --archive sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$sxmlfile"

srcml --archive sub/a.cpp -o sub/a.cpp.xml.gz && gunzip -c sub/a.cpp.xml.gz
check "$sxmlfile"
