#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test line endings output
define windows <<- 'STDOUT'
	sub/a.cpp: ASCII text, with CRLF line terminators
	STDOUT

define unix <<- 'STDOUT'
	sub/a.cpp: ASCII text
	STDOUT

define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"

# TODO: issue #999

# windows - clrf
srcml2src sub/a.cpp.xml --line-ending "Windows" -o sub/a.cpp && file sub/a.cpp
check 3<<< "$windows"

srcml2src --line-ending "Windows" sub/a.cpp.xml -o sub/a.cpp && file sub/a.cpp
check 3<<< "$windows"

srcml2src sub/a.cpp.xml --line-ending Windows -o sub/a.cpp && file sub/a.cpp
check 3<<< "$windows"

srcml2src --line-ending Windows sub/a.cpp.xml -o sub/a.cpp && file sub/a.cpp
check 3<<< "$windows"

srcml2src sub/a.cpp.xml --line-ending "windows" -o sub/a.cpp && file sub/a.cpp
check 3<<< "$windows"

srcml2src --line-ending "windows" sub/a.cpp.xml -o sub/a.cpp && file sub/a.cpp
check 3<<< "$windows"

srcml2src sub/a.cpp.xml --line-ending windows -o sub/a.cpp && file sub/a.cpp
check 3<<< "$windows"

srcml2src --line-ending windows sub/a.cpp.xml -o sub/a.cpp && file sub/a.cpp
check 3<<< "$windows"

# unix - lf
srcml2src sub/a.cpp.xml --line-ending "Unix" -o sub/a.cpp && file sub/a.cpp
check 3<<< "$unix"

srcml2src --line-ending "Unix" sub/a.cpp.xml -o sub/a.cpp && file sub/a.cpp
check 3<<< "$unix"

srcml2src sub/a.cpp.xml --line-ending Unix -o sub/a.cpp && file sub/a.cpp
check 3<<< "$unix"

srcml2src --line-ending Unix sub/a.cpp.xml -o sub/a.cpp && file sub/a.cpp
check 3<<< "$unix"

srcml2src sub/a.cpp.xml --line-ending "unix" -o sub/a.cpp && file sub/a.cpp
check 3<<< "$unix"

srcml2src --line-ending "unix" sub/a.cpp.xml -o sub/a.cpp && file sub/a.cpp
check 3<<< "$unix"

srcml2src sub/a.cpp.xml --line-ending unix -o sub/a.cpp && file sub/a.cpp
check 3<<< "$unix"

srcml2src --line-ending unix sub/a.cpp.xml -o sub/a.cpp && file sub/a.cpp
check 3<<< "$unix"


