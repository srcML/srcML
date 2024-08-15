#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file files_from.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# files from
define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$nestedfile"

createfile sub/a.cpp "
a;"

createfile sub/b.cpp "
b;"

createfile sub/filelistab "sub/a.cpp
sub/b.cpp
"

srcml --files-from "sub/filelistab" -o sub/both.xml
check sub/both.xml "$nestedfile"

srcml --files-from "sub/filelistab"
check "$nestedfile"
