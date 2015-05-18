#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	libsrcml Version Number REVISION
	srcml Version Number 9005
	libarchive Version 2008005
	STDOUT

srcml2src -V
check 3<<< "$output"

srcml2src --version
check 3<<< "$output"
