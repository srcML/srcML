#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file prefix_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# prefix extraction
define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:foo="foo.com" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
STDIN

xmlcheck "$input"
createfile sub/a.xml "$input"

# from file
srcml --show-prefix "http://www.srcML.org/srcML/src" sub/a.xml
check "\n"

srcml sub/a.xml --show-prefix "http://www.srcML.org/srcML/src"
check "\n"

srcml --show-prefix "http://www.srcML.org/srcML/cpp" sub/a.xml
check "cpp\n"

srcml sub/a.xml --show-prefix "http://www.srcML.org/srcML/cpp"
check "cpp\n"

srcml --show-prefix "foo.com" sub/a.xml
check "foo\n"

srcml sub/a.xml --show-prefix "foo.com"
check "foo\n"

srcml --show-prefix "nonexistent.com" sub/a.xml
check

srcml sub/a.xml --show-prefix "nonexistent.com"
check

# standard input
srcml --show-prefix "http://www.srcML.org/srcML/src" <<< "$input"
check "\n"

srcml --show-prefix="http://www.srcML.org/srcML/src" <<< "$input"
check "\n"

srcml --show-prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp\n"

srcml --show-prefix="http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp\n"

srcml --show-prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp\n"

srcml --show-prefix "http://www.cs.uakron.edu/~collard/foo" <<< "$input"
check

srcml --show-prefix="http://www.cs.uakron.edu/~collard/foo" <<< "$input"
check
