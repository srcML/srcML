#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
STDOUT

define fsrcml <<- 'STDOUT'
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
STDOUT

echo -n "" | src2srcml -l C++ --no-xml-declaration

check 3<<< "$srcml"

echo -n "" | src2srcml -l C++ --no-xml-declaration -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$srcml"

