#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# directory flag
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="bar"/>
	STDOUT
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="bar" filename="sub/a.cpp"/>
	INPUT
src2srcml sub/a.cpp -d "bar"
src2srcml sub/a.cpp --directory "bar"
src2srcml sub/a.cpp --directory="bar"
src2srcml -l C++ -d 'bar' -o sub/a.cpp.xml sfile1

validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -d 'bar' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



