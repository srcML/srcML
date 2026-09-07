#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file set_filename_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# filename flag
defineXML output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="foo.cpp"/>
STDOUT

createfile sub/a.cpp ""

srcml sub/a.cpp -f "foo.cpp"
check "$output"

srcml sub/a.cpp --filename "foo.cpp"
check "$output"

srcml sub/a.cpp --filename="foo.cpp"
check "$output"

srcml -l C++ -f 'foo.cpp' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$output"

srcml -f 'foo.cpp' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

# standard input
echo -n "" | srcml -l C++ -f foo.cpp
check "$output"

echo -n "" | srcml -l C++ --filename foo.cpp
check "$output"

echo -n "" | srcml -l C++ --filename=foo.cpp
check "$output"

# an empty value after the equals is rejected as a value, so the option is not
# set and the input filename is still an input file
defineXML emptyvalue <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
STDOUT

createfile sub/a.cpp "a;"

echo "a;" | srcml -l C++ --filename= sub/a.cpp
check "$emptyvalue"
