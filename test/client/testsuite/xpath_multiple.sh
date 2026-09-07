#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file xpath_multiple.sh
#
# @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# each --xpath on the command line is a separate transformation, applied in order,
# where the position of the input filename does not matter

defineXML output1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" item="1"><name>a</name></unit>

	</unit>
STDOUT

define oneline_a <<- 'STDOUT'
	a
STDOUT

defineXML srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

createfile sub/a.cpp.xml "$srcml"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml
check "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml --output-src
check "$oneline_a"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" < sub/a.cpp.xml
check "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output1"

# the input filename between the queries
srcml --xpath="/src:unit/src:expr_stmt" sub/a.cpp.xml --xpath="/src:expr_stmt/src:expr/src:name"
check "$output1"

srcml --xpath="/src:unit/src:expr_stmt" sub/a.cpp.xml --xpath="/src:expr_stmt/src:expr/src:name" --output-src
check "$oneline_a"

srcml --xpath="/src:unit/src:expr_stmt" sub/a.cpp.xml --xpath="/src:expr_stmt/src:expr/src:name" -o sub/b.cpp.xml
check sub/b.cpp.xml "$output1"

# the input filename before the queries
srcml sub/a.cpp.xml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name"
check "$output1"

srcml sub/a.cpp.xml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" --output-src
check "$oneline_a"

srcml sub/a.cpp.xml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" -o sub/b.cpp.xml
check sub/b.cpp.xml "$output1"

# more than two applications
srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr" --xpath="/src:expr/src:name" sub/a.cpp.xml
check "$output1"

srcml --xpath="/src:unit/src:expr_stmt" sub/a.cpp.xml --xpath="/src:expr_stmt/src:expr" --xpath="/src:expr/src:name"
check "$output1"

srcml sub/a.cpp.xml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr" --xpath="/src:expr/src:name"
check "$output1"
