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
src2srcml -e sub/a.cpp
checkv2 "$fsxmlfile" 4<<< "$deprecated_warning"

src2srcml --expression sub/a.cpp
checkv2 "$fsxmlfile" 4<<< "$deprecated_warning"

src2srcml sub/a.cpp -e
checkv2 "$fsxmlfile" 4<<< "$deprecated_warning"

src2srcml sub/a.cpp --expression
checkv2 "$fsxmlfile" 4<<< "$deprecated_warning"

src2srcml -l C++ --expression -o sub/a.cpp.xml < sub/a.cpp
checkv2 sub/a.cpp.xml "$sxmlfile" 4<<< "$deprecated_warning"

src2srcml --expression sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsxmlfile" 4<<< "$deprecated_warning"

# auto-detection
src2srcml sub/a.cpp
checkv2 "$fsxmlfile"

src2srcml -l C++ -o sub/a.cpp.xml < sub/a.cpp
checkv2 sub/a.cpp.xml "$sxmlfile"

src2srcml sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsxmlfile"


# within expression statement
src2srcml -e sub/b.cpp
checkv2 "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

src2srcml --expression sub/b.cpp
checkv2 "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

src2srcml sub/b.cpp -e
checkv2 "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

src2srcml sub/b.cpp --expression
checkv2 "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

src2srcml -l C++ --expression -o sub/b.cpp.xml < sub/b.cpp
checkv2 sub/b.cpp.xml "$expr_stmt_sxmlfile" 4<<< "$deprecated_warning"

src2srcml --expression sub/b.cpp -o sub/b.cpp.xml
checkv2 sub/b.cpp.xml "$expr_stmt_fsxmlfile" 4<<< "$deprecated_warning"

# auto-detection
src2srcml sub/b.cpp
checkv2 "$expr_stmt_fsxmlfile"

src2srcml -l C++ -o sub/b.cpp.xml < sub/b.cpp
checkv2 sub/b.cpp.xml "$expr_stmt_sxmlfile"

src2srcml sub/b.cpp -o sub/b.cpp.xml
checkv2 sub/b.cpp.xml "$expr_stmt_fsxmlfile"
