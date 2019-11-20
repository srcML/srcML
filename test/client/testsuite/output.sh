#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test output options
define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$sxmlfile"
xmlcheck "$xmlfile"
createfile sub/a.cpp "a;"

# output to scrML
srcml sub/a.cpp --output sub/a.cpp.xml
check sub/a.cpp.xml "$sxmlfile"

srcml sub/a.cpp --output=sub/a.cpp.xml
check sub/a.cpp.xml "$sxmlfile"

srcml sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$sxmlfile"

srcml -l C++ -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$sxmlfile"

# /dev/stdout causing issues in Windows tests
#srcml -l C++ - --output /dev/stdout < sub/a.cpp
#check "$xmlfile"

srcml -l C++ - -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$xmlfile"

# output to source code
define foutput <<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define fstuff <<- STDOUT
	a;
	STDOUT

createfile sub/a.cpp.xml "$foutput"

srcml sub/a.cpp.xml --output sub/a.cpp
check sub/a.cpp "$fstuff"

srcml sub/a.cpp.xml --output=sub/a.cpp
check sub/a.cpp "$fstuff"

srcml sub/a.cpp.xml -o sub/a.cpp
check sub/a.cpp "$fstuff"

srcml - -o sub/a.cpp <<< "$foutput"
check sub/a.cpp "$fstuff"

srcml -o sub/a.cpp <<< "$foutput"
check sub/a.cpp "$fstuff"

# srcml - --output /dev/stdout <<< "$foutput"
# check "$fstuff"

# srcml - --output=/dev/stdout <<< "$foutput"
# check "$fstuff"

# srcml - -o /dev/stdout <<< "$foutput"
# check "$fstuff"
