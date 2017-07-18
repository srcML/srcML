#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	libsrcml REVISION
	srcml REVISION
	STDOUT

srcml -V | head -n 2
check "$output"

srcml --version | head -n 2
check "$output"
