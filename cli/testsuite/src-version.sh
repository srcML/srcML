#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test src-version_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" version="1.0"/>
	STDOUT

echo -n "" | src2srcml -l C++ -s 1.0

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --src-version 1.0

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --src-version=1.0

check 3<<< "$output"

