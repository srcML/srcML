#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define xpath_error <<- 'STDOUT'
	Error Parsing: Start tag expected, '<' not found
	STDOUT

define xpath_empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/src"/>
	STDOUT

createfile sub/a.cpp.xml ""

srcml2src --xpath=src:unit sub/a.cpp.xml
check 3<<< "$xpath_empty" 4<<< "$xpath_error"

srcml2src -l C++ --xpath=src:unit < sub/a.cpp.xml
check 3<<< "$xpath_empty" 4<<< "$xpath_error"

srcml2src -l C++ --xpath=src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check 3<<< "$xpath_empty" 4<<< "$xpath_error"

srcml2src -l C++ --xpath=src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check 3<<< "$xpath_empty" 4<<< "$xpath_error"

