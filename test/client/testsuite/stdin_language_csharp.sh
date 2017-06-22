#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test language attribute C#
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C#"/>
	STDOUT

xmlcheck "$output"

echo -n "" | src2srcml -l C#
checkv2 "$output"

echo -n "" | src2srcml --language C#
checkv2 "$output"

echo -n "" | src2srcml --language=C#
checkv2 "$output"

