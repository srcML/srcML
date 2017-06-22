#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test encoding flag on a unit
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$fsrcml"
createfile sub/a.cpp ""

echo -n "" | src2srcml -l C++ -x "ISO-8859-1"
checkv2 "$srcml"

echo -n "" | src2srcml -l C++ --xml-encoding "ISO-8859-1"
checkv2 "$srcml"

echo -n "" | src2srcml -l C++ --xml-encoding="ISO-8859-1"
checkv2 "$srcml"

src2srcml sub/a.cpp -x "ISO-8859-1"
checkv2 "$fsrcml"

src2srcml sub/a.cpp --xml-encoding="ISO-8859-1"
checkv2 "$fsrcml"

src2srcml sub/a.cpp --xml-encoding "ISO-8859-1"
checkv2 "$fsrcml"

src2srcml -l C++ -x "ISO-8859-1" -o sub/a.cpp.xml < sub/a.cpp
checkv2 sub/a.cpp.xml "$srcml"

src2srcml -x "ISO-8859-1" sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsrcml"
