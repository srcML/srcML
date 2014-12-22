#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" revision="0.8.0" language="C++">
	INPUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

echo -n "" | src2srcml -l C++ --operator

check 3<<< "$fsrcml"

src2srcml --operator sub/a.cpp

check 3<<< "$fsrcml"

echo -n "" | src2srcml -l C++ --operator -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml --operator sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$fsrcml"

