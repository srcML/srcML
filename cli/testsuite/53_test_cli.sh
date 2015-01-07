#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

echo -n "" | src2srcml -l C++ --operator

check 3<<< "$output"

src2srcml --operator sub/a.cpp

check 3<<< "$fsrcml"

echo -n "" | src2srcml -l C++ --operator -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$output"
rmfile sub/a.cpp.xml

src2srcml --operator sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$fsrcml"
rmfile sub/a.cpp.xml

rmfile sub/a.cpp
