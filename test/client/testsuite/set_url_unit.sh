#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on standard in
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="bar"/>
	STDOUT

# test on file
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="bar" filename="sub/a.cpp"/>
	STDOUT

xmlcheck "$output"
xmlcheck "$fsrcml"
createfile sub/a.cpp ""

echo -n "" | src2srcml -l C++ --url bar
checkv2 "$output"

echo -n "" | src2srcml -l C++ --url=bar
checkv2 "$output"

src2srcml --url "bar" sub/a.cpp 
checkv2 "$fsrcml"

src2srcml --url "bar" sub/a.cpp 
checkv2 "$fsrcml"

src2srcml --url="bar" sub/a.cpp
checkv2 "$fsrcml"

src2srcml -l C++ --url 'bar' -o sub/a.cpp.xml sub/a.cpp
checkv2 sub/a.cpp.xml "$fsrcml"

src2srcml --url 'bar' sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsrcml"

src2srcml sub/a.cpp --url "bar"
checkv2 "$fsrcml"

src2srcml sub/a.cpp --url="bar"
checkv2 "$fsrcml"
