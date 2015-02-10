#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test xml_encoding_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

echo -n "" | src2srcml -l C++ -x "ISO-8859-1"

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --xml-encoding "ISO-8859-1"

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --xml-encoding="ISO-8859-1"

check 3<<< "$output"

