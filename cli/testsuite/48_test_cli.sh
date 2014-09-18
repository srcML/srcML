#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
srcml = <unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

fsrcml  <unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	STDOUT

src2srcml --no-xml-declaration
src2srcml -l C++ --no-xml-declaration
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --no-xml-declaration
validate(open(sub/a.cpp.xml 'r').read() fsrcml)


##

# no namespace declaration
