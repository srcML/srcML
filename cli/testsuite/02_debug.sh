#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test debug_namespace
define output <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" revision="0.8.0" language="C++"/>
	OUTPUT

echo -n "" | src2srcml -l C++ -g

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --debug

check 3<<< "$output"

