#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	libsrcml REVISION
	srcml REVISION
	libarchive 2.8.3
	STDOUT

srcml2src -V
check 3<<< "$output"

srcml2src --version
check 3<<< "$output"
