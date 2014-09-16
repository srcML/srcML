#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
	STDOUT

# separate
src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" 
echo -n "" | --xmlns:err="http://www.sdml.info/srcML/srcerr"


# multiple
echo -n "" | src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp"

echo -n "" | src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr"

echo -n "" | src2srcml -l C++ --debug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr"

