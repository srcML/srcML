#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test encoding flag on a unit
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$fsrcml"
createfile sub/a.cpp ""

echo -n "" | srcml -l C++ --xml-encoding "ISO-8859-1"
check "$srcml"

echo -n "" | srcml -l C++ --xml-encoding="ISO-8859-1"
check "$srcml"

srcml sub/a.cpp --xml-encoding="ISO-8859-1"
check "$fsrcml"

srcml sub/a.cpp --xml-encoding "ISO-8859-1"
check "$fsrcml"

srcml -l C++ --xml-encoding "ISO-8859-1" -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml --xml-encoding "ISO-8859-1" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"
