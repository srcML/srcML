#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test filename_url_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="bar" filename="foo"/>
	STDOUT
xmlcheck "$output"

echo -n "" | srcml -l C++ -f foo --url bar
check "$output"

echo -n "" | srcml -l C++ --url bar -f foo
check "$output"

