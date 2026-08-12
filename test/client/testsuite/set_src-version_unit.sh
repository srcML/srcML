#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file set_src-version_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test src-version_attribute
defineXML srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" version="1.0"/>
STDOUT

# file input
defineXML fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp" version="1.0"/>
STDOUT

createfile sub/a.cpp ""

srcml sub/a.cpp -s "1.0"
check "$fsrcml"

srcml sub/a.cpp --src-version "1.0"
check "$fsrcml"

srcml sub/a.cpp --src-version="1.0"
check "$fsrcml"

srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -s '1.0' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

# standard input
srcml -l C++ -s '1.0' -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

echo -n "" | srcml -l C++ -s 1.0
check "$srcml"

echo -n "" | srcml -l C++ --src-version 1.0
check "$srcml"

echo -n "" | srcml -l C++ --src-version=1.0
check "$srcml"

# an empty value after the equals is rejected as a value, so the option is not
# set and the input filename is still an input file
defineXML emptyvalue <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
STDOUT

createfile sub/a.cpp "a;"

echo "a;" | srcml -l C++ --src-version= sub/a.cpp
check "$emptyvalue"
