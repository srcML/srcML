#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# exit cleanup of generated files
trap "{ cleanup; }" EXIT

# check the result of the test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

# conduct test
echo -n "" | src2srcml -l C++

check 3<<< "$output"


createfile sub/test.cpp ""

echo -n "" | src2srcml -l C++ -o sub/stuff.cpp

check sub/stuff.cpp 3<<< "$output"

