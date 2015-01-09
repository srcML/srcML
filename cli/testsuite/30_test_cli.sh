#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# no namespace declaration

echo -n "" | src2srcml -l C++ --no-namespace-decl

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="0.8.0" language="C++"/>
	STDOUT
