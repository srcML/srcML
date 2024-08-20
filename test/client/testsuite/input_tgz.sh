#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file input_tgz.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test on compressed files with .tgz extension
define src <<- 'STDOUT'

	a;
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="archive/a.cpp.tgz">

	<unit revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$archive_output"
xmlcheck "$foutput"
xmlcheck "$output"

createfile archive/a.cpp "$src"
tar -czf archive/a.cpp.tgz archive/a.cpp

createfile list.txt "archive/a.cpp.tgz"

# src --> srcml
srcml archive/a.cpp.tgz -o archive/a.cpp.xml
check archive/a.cpp.xml "$foutput"

srcml archive/a.cpp.tgz
check "$foutput"

srcml -l C++ < archive/a.cpp.tgz
check "$output"

srcml -l C++ -o archive/a.cpp.xml < archive/a.cpp.tgz
check archive/a.cpp.xml "$output"

# files from
srcml --files-from list.txt
check "$archive_output"

srcml --files-from list.txt -o archive/list.xml
check archive/list.xml "$archive_output"

# files from empty (not necessary - archive format)

rmfile list.txt
rmfile archive/a.cpp
rmfile archive/a.cpp.tgz

# srcml --> src
srcml archive/a.cpp.xml
check "$src"

srcml archive/a.cpp.xml -o archive/a.cpp
check archive/a.cpp "$src"

srcml < archive/a.cpp.xml
check "$src"

srcml -o archive/a.cpp < archive/a.cpp.xml
check archive/a.cpp "$src"
