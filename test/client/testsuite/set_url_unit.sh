#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file set_url_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test on standard in
defineXML output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="bar"/>
STDOUT

# test on file
defineXML fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="bar" filename="sub/a.cpp"/>
STDOUT

createfile sub/a.cpp ""

echo -n "" | srcml -l C++ --url bar
check "$output"

echo -n "" | srcml -l C++ --url=bar
check "$output"

srcml --url "bar" sub/a.cpp
check "$fsrcml"

srcml --url "bar" sub/a.cpp
check "$fsrcml"

srcml --url="bar" sub/a.cpp
check "$fsrcml"

srcml -l C++ --url 'bar' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"

srcml --url 'bar' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml sub/a.cpp --url "bar"
check "$fsrcml"

srcml sub/a.cpp --url="bar"
check "$fsrcml"

# an empty value after the equals is rejected as a value, so the option is not
# set and the input filename is still an input file
defineXML emptyvalue <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
STDOUT

createfile sub/a.cpp "a;"

echo "a;" | srcml -l C++ --url= sub/a.cpp
check "$emptyvalue"
