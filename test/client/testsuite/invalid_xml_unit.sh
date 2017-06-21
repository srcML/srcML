#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# xml error on invalid input (single file) test
define info_single <<- 'STDOUT'
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:cpp="http://www.srcML.org/srcML/cpp"
	encoding="UTF-8"
	language="C++"
	STDOUT

define illformed <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit
	INPUT

createfile xml_error/illformed.xml "$illformed"

# ok
srcml2src xml_error/illformed.xml --info
check 3<<< "$info_single"

srcml2src --info xml_error/illformed.xml
check 3<<< "$info_single"

define output <<- 'STDOUT'
	
	a;
	STDOUT

define xml_error <<- 'STDERR'
	Error Parsing: expected '>'
	STDERR

# bad
srcml2src xml_error/illformed.xml
check 3<<< "$output" 4<<< "$xml_error"
