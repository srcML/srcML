#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test expression_mode
define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr><name>a</name></expr></unit>
	STDOUT

define fsxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr><name>a</name></expr></unit>
	STDOUT

define expr_stmt_sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>
	STDOUT

define expr_stmt_fsxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$sxmlfile"
xmlcheck "$fsxmlfile"
xmlcheck "$expr_stmt_sxmlfile"
xmlcheck "$expr_stmt_fsxmlfile"

createfile sub/a.cpp "a"
createfile sub/b.cpp "b;"

# expression only
srcml sub/a.cpp
check "$fsxmlfile"

srcml sub/a.cpp
check "$fsxmlfile"

srcml sub/a.cpp
check "$fsxmlfile"

srcml sub/a.cpp
check "$fsxmlfile"

srcml -l C++ -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$sxmlfile"

srcml sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsxmlfile"

# auto-detection
srcml sub/a.cpp
check "$fsxmlfile"

srcml -l C++ -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$sxmlfile"

srcml sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsxmlfile"

# within expression statement
srcml sub/b.cpp
check "$expr_stmt_fsxmlfile"

srcml sub/b.cpp
check "$expr_stmt_fsxmlfile"

srcml sub/b.cpp
check "$expr_stmt_fsxmlfile"

srcml sub/b.cpp
check "$expr_stmt_fsxmlfile"

srcml -l C++ -o sub/b.cpp.xml < sub/b.cpp
check sub/b.cpp.xml "$expr_stmt_sxmlfile"

srcml sub/b.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml "$expr_stmt_fsxmlfile"

# auto-detection
srcml sub/b.cpp
check "$expr_stmt_fsxmlfile"

srcml -l C++ -o sub/b.cpp.xml < sub/b.cpp
check sub/b.cpp.xml "$expr_stmt_sxmlfile"

srcml sub/b.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml "$expr_stmt_fsxmlfile"
