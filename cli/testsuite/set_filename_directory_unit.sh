#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test filename_directory_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" directory="bar" filename="foo"/>
	STDOUT

echo -n "" | src2srcml -l C++ -f foo -d bar

check 3<<< "$output"

echo -n "" | src2srcml -l C++ -d bar -f foo

check 3<<< "$output"

