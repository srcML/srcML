#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# xml error on invalid input archive test
define illformed_archive_mulit <<- 'INPUT'
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

define illformed_archive_single <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	</unit>

	</unit>
	INPUT

# expected output
define info_archive <<- 'STDOUT'
	xmlns="http://www.sdml.info/srcML/src"
	encoding="UTF-8"
	STDOUT

define output <<- 'STDOUT'
	
	a;
	STDOUT

define xml_archive_error <<- 'STDERR'
	srcml: Extra content at the end of the document in 'xml_error/illformedarchive_multi.xml'
	STDERR

createfile xml_error/illformedarchive_multi.xml "$illformed_archive_mulit"
createfile xml_error/illformedarchive_single.xml "$illformed_archive_single"

# ok
srcml2src xml_error/illformedarchive_multi.xml --info
check 3<<< "$info_archive"

srcml2src xml_error/illformedarchive_single.xml --info
check 3<<< "$info_archive"

srcml2src --info xml_error/illformedarchive_multi.xml
check 3<<< "$info_archive"

srcml2src --info xml_error/illformedarchive_single.xml
check 3<<< "$info_archive"

srcml2src --info < xml_error/illformedarchive_multi.xml
check 3<<< "$info_archive"

srcml2src --info < xml_error/illformedarchive_single.xml
check 3<<< "$info_archive"


# bad
# TODO: issue #1039
srcml2src xml_error/illformedarchive_multi.xml --units
check 3<<< "$output" 4<<< "$xml_archive_error"

srcml2src xml_error/illformedarchive_single.xml --units
check 3<<< "$output" 4<<< "$xml_archive_error"

srcml2src --units xml_error/illformedarchive_multi.xml
check 3<<< "$output" 4<<< "$xml_archive_error"

srcml2src --units xml_error/illformedarchive_single.xml
check 3<<< "$output" 4<<< "$xml_archive_error"

srcml2src --units < xml_error/illformedarchive_multi.xml
check 3<<< "$output" 4<<< "$xml_archive_error"

srcml2src --units < xml_error/illformedarchive_single.xml
check 3<<< "$output" 4<<< "$xml_archive_error"
