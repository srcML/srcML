#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test language attribute Objective-C
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="Objective-C"/>
	STDOUT

echo -n "" | src2srcml -l Objective-C

check 3<<< "$output"

echo -n "" | src2srcml --language Objective-C

check 3<<< "$output"

echo -n "" | src2srcml --language=Objective-C

check 3<<< "$output"

