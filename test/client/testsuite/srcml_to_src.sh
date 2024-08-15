#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file srcml_to_src.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test srcml_extraction

define src <<< "
a;"

define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDIN

xmlcheck "$input"

srcml <<< "$input"
check "$src"

createfile sub/a.cpp.xml "$input"

srcml sub/a.cpp.xml
check "$src"
