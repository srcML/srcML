#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# no namespace declaration

echo -n "" | src2srcml -l C++ --no-namespace-decl
INPUT

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++"/>
	STDOUT

