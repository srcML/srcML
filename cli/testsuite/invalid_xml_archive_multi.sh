#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# xml error on invalid input (multi file archive) test
define info_archive <<- 'STDOUT'
	xmlns="http://www.sdml.info/srcML/src"
	encoding="UTF-8"
	STDOUT

define illformed_archive <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	</unit>

	</unit>
	INPUT

createfile xml_error/illformedarchive.xml "$illformed_archive"

# ok
srcml2src --info xml_error/illformedarchive.xml
check 3<<< "$info_archive"

define output <<- 'STDOUT'
	
	a;
	STDOUT

define xml_archive_error <<- 'STDERR'
	srcml: Extra content at the end of the document in 'xml_error/illformedarchive.xml'
	STDERR

# bad
# TODO: issue #1039
srcml2src --units xml_error/illformedarchive.xml
check 3<<< "$output" 4<<< "$xml_archive_error"

