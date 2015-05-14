#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test filename_url_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="bar" filename="foo"/>
	STDOUT

echo -n "" | src2srcml -l C++ -f foo --url bar

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --url bar -f foo

check 3<<< "$output"

