#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename=sub/a.cpp/>
	INPUT

src2srcml sub/a.cpp -l "C"
src2srcml sub/a.cpp --language "C"
src2srcml sub/a.cpp --language="C"
src2srcml -l 'C' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -l 'C' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



