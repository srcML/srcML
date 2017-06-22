#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test output options
define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$sxmlfile"
xmlcheck "$xmlfile"
createfile sub/a.cpp "a;"

# output to scrML
src2srcml sub/a.cpp --output sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$sxmlfile"

src2srcml sub/a.cpp --output=sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$sxmlfile"

src2srcml sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$sxmlfile"

src2srcml -l C++ -o sub/a.cpp.xml sub/a.cpp
checkv2 sub/a.cpp.xml "$sxmlfile"

src2srcml -l C++ - --output /dev/stdout < sub/a.cpp
checkv2 "$xmlfile"

src2srcml -l C++ - -o sub/a.cpp.xml < sub/a.cpp
checkv2 sub/a.cpp.xml "$xmlfile"

# output to source code
define foutput <<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define fstuff <<- STDOUT
	a;
	STDOUT

createfile sub/a.cpp.xml "$foutput"

srcml2src sub/a.cpp.xml --output sub/a.cpp
checkv2 sub/a.cpp "$fstuff"

srcml2src sub/a.cpp.xml --output=sub/a.cpp
checkv2 sub/a.cpp "$fstuff"

srcml2src sub/a.cpp.xml -o sub/a.cpp
checkv2 sub/a.cpp "$fstuff"

srcml2src - -o sub/a.cpp <<< "$foutput"
checkv2 sub/a.cpp "$fstuff"

srcml2src -o sub/a.cpp <<< "$foutput"
checkv2 sub/a.cpp "$fstuff"

srcml2src - --output /dev/stdout <<< "$foutput"
checkv2 "$fstuff"

srcml2src - --output=/dev/stdout <<< "$foutput"
checkv2 "$fstuff"

srcml2src - -o /dev/stdout <<< "$foutput"
checkv2 "$fstuff"
