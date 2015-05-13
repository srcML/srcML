#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:err="http://www.srcML.org/srcML/srcerr" revision="REVISION" language="C++"/>
	STDOUT

# separate
echo -n "" | src2srcml -l C++ --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" --xmlns:err="http://www.srcML.org/srcML/srcerr"
check 3<<< "$output"

# multiple
echo -n "" | src2srcml -l C++ --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check 3<<< "$output"

echo -n "" | src2srcml -l C++ --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:err="http://www.srcML.org/srcML/srcerr"
check 3<<< "$output"

echo -n "" | src2srcml -l C++ --debug --xmlns:cpp="http://www.srcML.org/srcML/cpp" --xmlns:err="http://www.srcML.org/srcML/srcerr"
check 3<<< "$output"
