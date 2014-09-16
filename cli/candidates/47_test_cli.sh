#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

 output <<- 'STDOUT'

	version="1.0" encoding="UTF-8" standalone="yes"?>
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	STDOUT<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp/>
	INPUT
src2srcml --xmlns:src=http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp" sub/a.cpp
src2srcml -l C++ --xmlns:src=http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --xmlns:src=http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



##

# no xml declaration
