#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file output_src_archive.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

define srca <<- 'STDOUT'
	a;
	STDOUT

define srcb <<- 'STDOUT'
	b;
	STDOUT

define srcab <<- 'STDOUT'
	a;
	b;
	STDOUT

# multi archive
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

# src --> srcml : input srcml single file
xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

# have to get null byte into test case result
if [[ "$OSTYPE" == 'msys' ]]; then
    printf "a;\r\n\0b;\r\n" > expected_output
else
    printf "a;\n\0b;\n" > expected_output
fi

# have to get null byte into test case result
if [[ "$OSTYPE" == 'msys' ]]; then
    printf "a;\r\nb;\r\n" > expected_output_newline
else
    printf "a;\nb;\n" > expected_output_newline
fi

srcml --output-src sub/a.cpp.xml > ab
check_file ab expected_output_newline
rmfile ab

srcml --output-src sub/a.cpp.xml --print0 > ab
check_file ab expected_output
rmfile ab

srcml --output-src sub/a.cpp.xml -Z > ab
check_file ab expected_output
rmfile ab

srcml -S sub/a.cpp.xml > ab
check_file ab expected_output_newline
rmfile ab

srcml -S sub/a.cpp.xml --print0 > ab
check_file ab expected_output
rmfile ab

srcml -S sub/a.cpp.xml -Z > ab
check_file ab expected_output
rmfile ab

srcml -U 1 --output-src sub/a.cpp.xml
check "$srca"

srcml -U 1 -S sub/a.cpp.xml
check "$srca"

srcml -U 2 --output-src sub/a.cpp.xml
check "$srcb"

srcml -U 2 -S sub/a.cpp.xml
check "$srcb"

cat sub/a.cpp.xml | srcml -S > ab
check_file ab expected_output_newline
rmfile ab

cat sub/a.cpp.xml | srcml -S --print0 > ab
check_file ab expected_output
rmfile ab

cat sub/a.cpp.xml | srcml -S -Z > ab
check_file ab expected_output
rmfile ab

srcml -U 1 -S < sub/a.cpp.xml
check "$srca"

srcml -U 2 -S < sub/a.cpp.xml
check "$srcb"

srcml -S sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srca"

srcml -S -U 1 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srca"

srcml -S -U 2 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srcb"

srcml -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -S -U 1 -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 1 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 2 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srcb"

srcml -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 1 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 2 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srcb"

# single archive
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

# srcml --> src : input srcml single file
xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

srcml --output-src sub/a.cpp.xml
check "$srca"

srcml -S sub/a.cpp.xml
check "$srca"

srcml -U 1 --output-src sub/a.cpp.xml
check "$srca"

srcml -U 1 -S sub/a.cpp.xml
check "$srca"

srcml -S < sub/a.cpp.xml
check "$srca"

srcml -U 1 -S < sub/a.cpp.xml
check "$srca"

srcml -S sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srca"

srcml -S -U 1 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srca"

srcml -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -S -U 1 -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 1 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 1 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srca"
