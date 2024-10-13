#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file prefix_archive.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# archive of one unit
define input <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit revision="REVISION" language="C++" xmlns:cpp="http://www.srcML.org/srcML/cpp" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
INPUT

xmlcheck "$input"
createfile sub/a.xml "$input"

# from a file
srcml -U 1 -X --show-prefix "http://www.srcML.org/srcML/cpp" sub/a.xml
check "cpp
"

srcml sub/a.xml -U 1 -X --show-prefix "http://www.srcML.org/srcML/cpp"
check "cpp
"

srcml --show-prefix "http://www.srcML.org/srcML/src" sub/a.xml
check "
"

srcml sub/a.xml --show-prefix "http://www.srcML.org/srcML/src"
check "
"

srcml --show-prefix "foo.com" sub/a.xml
check "foo
"

srcml sub/a.xml --show-prefix "foo.com"
check "foo
"

srcml --show-prefix "nonexistent.com" sub/a.xml
check

srcml sub/a.xml --show-prefix "nonexistent.com"
check

# standard in
srcml -U 1 -X --show-prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp
"

srcml -U 1 -X --show-prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp
"

srcml -U 1 -X --show-prefix="http://www.srcML.org/srcML/cpp" <<< "$input"
check "cpp
"

srcml --show-prefix "http://www.srcML.org/srcML/src" <<< "$input"
check "
"

srcml --show-prefix "http://www.srcML.org/srcML/src" <<< "$input"
check "
"

srcml --show-prefix="http://www.srcML.org/srcML/src" <<< "$input"
check "
"

# archive of multiple units
define archive_multi <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
INPUT

createfile sub/archive_multi.xml "$archive_multi"

srcml -U 1 -X --show-prefix "http://www.srcML.org/srcML/cpp" sub/archive_multi.xml
check ""

srcml sub/archive_multi.xml -U 1 -X --show-prefix "http://www.srcML.org/srcML/cpp"
check ""

srcml --show-prefix "http://www.srcML.org/srcML/src" sub/archive_multi.xml
check "
"

srcml sub/archive_multi.xml --show-prefix "http://www.srcML.org/srcML/src"
check "
"

srcml --show-prefix "foo.com" sub/archive_multi.xml
check "foo
"

srcml sub/archive_multi.xml --show-prefix "foo.com"
check "foo
"

srcml --show-prefix "nonexistent.com" sub/archive_multi.xml
check

srcml sub/archive_multi.xml --show-prefix "nonexistent.com"
check

# standard in
srcml -U 1 -X --show-prefix "http://www.srcML.org/srcML/cpp" <<< "$archive_multi"
check ""

srcml -U 1 -X --show-prefix="http://www.srcML.org/srcML/cpp" <<< "$archive_multi"
check ""

srcml --show-prefix "http://www.srcML.org/srcML/src" <<< "$archive_multi"
check "
"

srcml --show-prefix="http://www.srcML.org/srcML/src" <<< "$archive_multi"
check "
"
