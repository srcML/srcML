#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file remote_suffix_file.sh
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test a position suffix on a remote source file

defineXML output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

defineXML empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp"/>
STDOUT

# the only line of the remote file
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp:1
check "$output"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp:1-1 --output-src
check "a;\n"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp:1- --output-src
check "a;\n"

# a line past the end of the remote file has no source
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp:9
check "$empty"

# lines and columns start at one
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp:0
check_exit 1
