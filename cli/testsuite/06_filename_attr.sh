#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test filename_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="foo"/>
	STDOUT

echo -n "" | src2srcml -l C++ -f foo

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --filename foo

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --filename="foo"

check 3<<< "$output"

