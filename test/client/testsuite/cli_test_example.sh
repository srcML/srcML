#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file cli_test_example.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# check the result of the test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"/>
	STDOUT
xmlcheck "$output"

# conduct test
echo -n "" | srcml -l C++
check "$output"

createfile sub/test.cpp ""

echo -n "" | srcml -l C++ -o sub/stuff.cpp
check sub/stuff.cpp "$output"
