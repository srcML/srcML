#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file set_metadata_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test metadata options with files
define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="sub" filename="a.cpp" version="1.2">
	</unit>
STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.srcML.org/srcML/src">

	<src:unit revision="REVISION" language="C++" filename="sub/a.cpp">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	<src:unit revision="REVISION" language="C++" filename="sub/b.cpp">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	</src:unit>
STDOUT

xmlcheck "$sxmlfile1"
xmlcheck "$nestedfile"
createfile sub/a.cpp.xml "$sxmlfile1"

srcml sub/a.cpp.xml --show-language
check "C++\n"

srcml sub/a.cpp.xml --show-url
check "sub\n"

srcml sub/a.cpp.xml --show-filename
check "a.cpp\n"

srcml sub/a.cpp.xml --show-src-version
check "1.2\n"

srcml sub/a.cpp.xml --show-encoding
check "UTF-8\n"

srcml --show-unit-count sub/a.cpp.xml
check "1\n"

createfile sub/nested.cpp.xml "$nestedfile"

srcml --show-unit-count sub/nested.cpp.xml
check "2\n"
