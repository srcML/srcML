#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	libsrcml REVISION
	srcml REVISION
	STDOUT

srcml2src -V | head -n 2
check 3<<< "$output"

srcml2src --version | head -n 2
check 3<<< "$output"
