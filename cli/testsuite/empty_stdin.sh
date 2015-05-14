#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test simple
echo -n "" | src2srcml -l C++

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

