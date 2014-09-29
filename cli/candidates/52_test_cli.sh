#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
createfile sub/a.cpp ""

define sfilelit <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" language="C++" filename="sub/a.cpp"/>
	STDOUT

echo -n "" | src2srcml -l C++ --literal

check sub/a.cpp.xml 3<<< "$sfilelit"

src2srcml --literal sub/a.cpp 

check sub/a.cpp.xml 3<<< "$sfilelit"

echo -n "" | src2srcml -l C++ --literal -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfilelit"

src2srcml --literal sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfilelit"

