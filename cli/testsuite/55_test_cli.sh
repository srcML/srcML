#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" revision="0.8.0" language="C++">
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

echo -n "" | src2srcml -l C++ --literal --operator

check 3<<< "$srcml"

src2srcml --literal --operator sub/a.cpp

check 3<<< "$srcml"

echo -n "" | src2srcml -l C++ --literal --operator -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfile"

src2srcml --literal --operator sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfile"

src2srcml -l C++ --operator --literal sfile srcml)

src2srcml --operator --literal sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --operator --literal -o sub/a.cpp.xml sfie
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --operator --literal sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



