#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test language_attribute_c
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C"/>
	STDOUT

xmlcheck "$output"

echo -n "" | srcml -l "C"
check "$output"

echo -n "" | srcml --language "C"
check "$output"

echo -n "" | srcml --language="C"
check "$output"

