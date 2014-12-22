#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# filename flag
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes">
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="foo.cpp"/>
	INPUT

define output <<- 'STDOUT'
	f<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="foo.cpp"/>
	INPUT

src2srcml sub/a.cpp -f "foo.cpp"
src2srcml sub/a.cpp --filename "foo.cpp"
src2srcml sub/a.cpp --filename="foo.cpp"
src2srcml -l C++ -f 'foo.cpp' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -f 'foo.cpp' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



