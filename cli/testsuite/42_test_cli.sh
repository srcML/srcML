#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# version flag
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes">
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" version="1.0"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp version="1.0"/>
	INPUT
src2srcml sub/a.cpp -s "1.0"
src2srcml sub/a.cpp --src-version "1.0"
src2srcml sub/a.cpp --src-version="1.0"

src2srcml -l C++ -s '1.0' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -s '1.0' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



##

# xml encoding flag
