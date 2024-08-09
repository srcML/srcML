#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file list_archive_single.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	XML encoding: UTF-8
	    1  C++     1 aa2a72b26cf958d8718a2e9bc6b84679a81d54cb a.cpp
	units: 1
	LOC: 1
	STDOUT

xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

# test --list on input archives
srcml --list sub/a.cpp.xml
check "$output"

srcml -L sub/a.cpp.xml
check "$output"

srcml --list < sub/a.cpp.xml
check "$output"

srcml -L < sub/a.cpp.xml
check "$output"
