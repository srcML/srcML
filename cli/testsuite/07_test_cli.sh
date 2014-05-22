#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test filename_attribute_path
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="bar/foo"/>
	STDOUT

echo -n "" | src2srcml -l C++ -f bar/foo

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --filename bar/foo

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --filename=bar/foo

check 3<<< "$output"

