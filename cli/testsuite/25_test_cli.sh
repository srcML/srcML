#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

# separate
echo -n "" | src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src"

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --xmlns:cpp="http://www.sdml.info/srcML/cpp"

check 3<<< "$output"

# multiple

echo -n "" | src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp"

check 3<<< "$output"

