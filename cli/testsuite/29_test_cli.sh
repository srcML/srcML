#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# no xml declaration
echo -n "" | src2srcml -l C++ --no-xml-declaration

check 3<<- 'STDOUT'
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
