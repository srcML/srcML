#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# Testing for verbose

define sfile <<< "a;"

define sxml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define vfoutput <<- 'STDERR'
	Source encoding:  (null)
	XML encoding:  UTF-8
	    1 sub/a.cpp	C++	1	10	HASH

	Translated: 1	Skipped: 0	Error: 0	Total: 1
	STDERR

define voutput <<- 'STDERR'
	Source encoding:  (null)
	XML encoding:  UTF-8
	    1	C++	1	10	HASH

	Translated: 1	Skipped: 0	Error: 0	Total: 1
	STDERR

createfile sub/a.cpp "$sfile"

createfile sub/a.cpp.xml "$sxmlfile"
 
 # src to srcml
srcml --verbose sub/a.cpp
check 3<<< "$sxmlfile" 4<<< "$vfoutput"

srcml --verbose sub/a.cpp -o sub/c.cpp.xml
check sub/c.cpp.xml 3<<< "$sxmlfile" 4<<< "$vfoutput"
rmfile sub/c.cpp.xml

srcml --verbose -l C++ < sub/a.cpp
check 3<<< "$sxml" 4<<< "voutput"

srcml --verbose -l C++ -o sub/c.cpp.xml < sub/a.cpp
check 3<<< "$sxml" 4<<< "$voutput"
rmfile sub/c.cpp.xml

# srcml to src
srcml --verbose sub/a.cpp.xml
check 3<<< "$sfile"

srcml --verbose sub/a.cpp.xml -o sub/c.cpp
check_null
rmfile sub/c.cpp

srcml --verbose -l C++ < sub/a.cpp.xml
check 3<<< "$sfile"

srcml --verbose -l C++ -o sub/c.cpp < sub/a.cpp.xml
check_null
rmfile sub/c.cpp


rmfile sub/a.cpp
rmfile sub/a.cpp.xml
