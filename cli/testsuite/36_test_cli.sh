#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# empty with debug
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" revision="0.8.0" language="C++"/>
	STDOUT

define output <<- 'STDOUT'
	?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

src2srcml sub/a.cpp -g fsrcml

src2srcml sub/a.cpp --debug fsrcml

src2srcml -l C++ -g -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -g sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



