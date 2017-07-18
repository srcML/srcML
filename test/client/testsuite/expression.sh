#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test expression_mode
define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr><name>a</name></expr></unit>
	STDOUT

define fsxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr><name>a</name></expr></unit>
	STDOUT

define expr_stmt_sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>
	STDOUT

define expr_stmt_fsxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$sxmlfile"
xmlcheck "$fsxmlfile"
xmlcheck "$expr_stmt_sxmlfile"
xmlcheck "$expr_stmt_fsxmlfile"


createfile sub/a.cpp "a"
createfile sub/b.cpp "b;"

# Deprecated warning message
define deprecated_warning <<- 'STDERR'
	WARNING srcml: use of option --expression or -e is deprecated
STDERR

# expression only
srcml -e sub/a.cpp
check "$fsxmlfile" "$deprecated_warning"

srcml --expression sub/a.cpp
check "$fsxmlfile" "$deprecated_warning"

srcml sub/a.cpp -e
check "$fsxmlfile" "$deprecated_warning"

srcml sub/a.cpp --expression
check "$fsxmlfile" "$deprecated_warning"

srcml -l C++ --expression -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$sxmlfile" "$deprecated_warning"

srcml --expression sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsxmlfile" "$deprecated_warning"

# auto-detection
srcml sub/a.cpp
check "$fsxmlfile"

srcml -l C++ -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$sxmlfile"

srcml sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsxmlfile"


# within expression statement
srcml -e sub/b.cpp
check "$expr_stmt_fsxmlfile" "$deprecated_warning"

srcml --expression sub/b.cpp
check "$expr_stmt_fsxmlfile" "$deprecated_warning"

srcml sub/b.cpp -e
check "$expr_stmt_fsxmlfile" "$deprecated_warning"

srcml sub/b.cpp --expression
check "$expr_stmt_fsxmlfile" "$deprecated_warning"

srcml -l C++ --expression -o sub/b.cpp.xml < sub/b.cpp
check sub/b.cpp.xml "$expr_stmt_sxmlfile" "$deprecated_warning"

srcml --expression sub/b.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml "$expr_stmt_fsxmlfile" "$deprecated_warning"

# auto-detection
srcml sub/b.cpp
check "$expr_stmt_fsxmlfile"

srcml -l C++ -o sub/b.cpp.xml < sub/b.cpp
check sub/b.cpp.xml "$expr_stmt_sxmlfile"

srcml sub/b.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml "$expr_stmt_fsxmlfile"
