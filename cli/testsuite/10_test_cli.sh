#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test src-version_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" version="1.0"/>
	STDOUT

echo -n "" | src2srcml -l C++ -s 1.0

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --src-version 1.0

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --src-version=1.0

check 3<<< "$output"

