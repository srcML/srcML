#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test literal and operator combinations

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT

define sfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

echo -n "" | src2srcml -l C++ --literal --operator

check 3<<< "$srcml"

src2srcml --literal --operator sub/a.cpp

check 3<<< "$sfile"

echo -n "" | src2srcml -l C++ --literal --operator -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$srcml"
rmfile sub/a.cpp.xml

src2srcml --literal --operator sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfile"
rmfile sub/a.cpp.xml

src2srcml -l C++ --operator --literal < sub/a.cpp

check 3<<< "$srcml"

src2srcml --operator --literal sub/a.cpp

check 3<<< "$sfile"

src2srcml -l C++ --operator --literal -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"
rmfile sub/a.cpp.xml

rmfile sub/a.cpp
