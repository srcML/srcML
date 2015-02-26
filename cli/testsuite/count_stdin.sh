#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test --show-unit-count on standard input file
define src <<- 'STDOUT'
	a;
	STDOUT

define output <<- 'STDOUT'
	1
	STDOUT

createfile sub/a.cpp "$src"

src2srcml --show-unit-count -l C++ < sub/a.cpp
check 3<<< "$output"

src2srcml -l C++ --show-unit-count < sub/a.cpp
check 3<<< "$output"
