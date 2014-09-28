#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	STDOUT

f<?xml vrsion="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
	STDOUT
src2srcml sub/a.cpp -x --encoding "ISO-8859-1"
src2srcml sub/a.cpp -x --encoding "ISO-8859-1"
src2srcml sub/a.cpp -x --encoding "ISO-8859-1"
src2srcml -l C++ -x 'ISO-8859-1' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -x 'ISO-8859-1' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



##

# text encoding flag
