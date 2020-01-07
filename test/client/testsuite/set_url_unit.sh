#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on standard in
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="bar"/>
	STDOUT

# test on file
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="bar" filename="sub/a.cpp"/>
	STDOUT

xmlcheck "$output"
xmlcheck "$fsrcml"
createfile sub/a.cpp ""

echo -n "" | srcml -l C++ --url bar
check "$output"

echo -n "" | srcml -l C++ --url=bar
check "$output"

srcml --url "bar" sub/a.cpp
check "$fsrcml"

srcml --url "bar" sub/a.cpp
check "$fsrcml"

srcml --url="bar" sub/a.cpp
check "$fsrcml"

srcml -l C++ --url 'bar' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"

srcml --url 'bar' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml sub/a.cpp --url "bar"
check "$fsrcml"

srcml sub/a.cpp --url="bar"
check "$fsrcml"
