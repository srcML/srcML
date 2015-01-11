#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# test text encoding flag

define sfile1 <<< "a;\n"

define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="a.cpp">
	</unit>
	STDOUT

createfile sub/a.cpp "$sfile1"
createfile sub/a.cpp.xml "$sxmlfile1"

srcml --get-src-encoding "ISO-8859-1" sub/a.cpp

check 3<<< "$sfile1"

srcml sub/a.cpp.xml --get-src-encoding "ISO-8859-1"

check 3<<< "$sfile1"

srcml --get-src-encoding "ISO-8859-1" sub/a.cpp.xml

check 3<<< "$sfile1"

srcml --get-src-encoding "ISO-8859-1" -o sub/a.cpp sub/a.cpp.xml
check sub/a.cpp 3<<< "$sfile1"

srcml --get-src-encoding "ISO-8859-1" sub/a.cpp.xml -o sub/a.cpp

check sub/a.cpp 3<<< "$sfile1"
