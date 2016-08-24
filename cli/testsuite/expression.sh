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
	srcml: use of option --expression or -e is deprecated
STDERR

# expression only
src2srcml -e sub/a.cpp
check 3<<< "$fsxmlfile" 4<<< "$deprecated_warning"

src2srcml --expression sub/a.cpp
check 3<<< "$fsxmlfile" 4<<< "$deprecated_warning"

src2srcml sub/a.cpp -e
check 3<<< "$fsxmlfile" 4<<< "$deprecated_warning"

src2srcml sub/a.cpp --expression
check 3<<< "$fsxmlfile" 4<<< "$deprecated_warning"

src2srcml -l C++ --expression -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$sxmlfile" 4<<< "$deprecated_warning"

src2srcml --expression sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsxmlfile" 4<<< "$deprecated_warning"

# auto-detection
src2srcml sub/a.cpp
check 3<<< "$fsxmlfile"

src2srcml -l C++ -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsxmlfile"


# within expression statement
src2srcml -e sub/b.cpp
check 3<<< "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

src2srcml --expression sub/b.cpp
check 3<<< "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

src2srcml sub/b.cpp -e
check 3<<< "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

src2srcml sub/b.cpp --expression
check 3<<< "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

src2srcml -l C++ --expression -o sub/b.cpp.xml < sub/b.cpp
check sub/b.cpp.xml 3<<< "$expr_stmt_sxmlfile" 4<<< "$deprecated_warning"

src2srcml --expression sub/b.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

# auto-detection
src2srcml sub/b.cpp
check 3<<< "$expr_stmt_fsxmlfile"

src2srcml -l C++ -o sub/b.cpp.xml < sub/b.cpp
check sub/b.cpp.xml 3<<< "$expr_stmt_sxmlfile"

src2srcml sub/b.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$expr_stmt_fsxmlfile"
