#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# xml error on invalid input archive test
define illformed_archive_mulit <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	</unit>

	</unit>
	INPUT

define illformed_archive_single <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	</unit>

	</unit>
	INPUT

# expected output
define info_archive <<- 'STDOUT'
	xmlns="http://www.srcML.org/srcML/src"
	encoding="UTF-8"
	STDOUT

define output <<- 'STDOUT'

	a;
	STDOUT

define units3 <<- 'STDOUT'
	3
	STDOUT

define units1 <<- 'STDOUT'
	1
	STDOUT

define xml_archive_error <<- 'STDERR'
	srcml: 17:1 Extra content at the end of the document
	STDERR

createfile xml_error/illformedarchive_multi.xml "$illformed_archive_mulit"
createfile xml_error/illformedarchive_single.xml "$illformed_archive_single"

# bad
srcml xml_error/illformedarchive_multi.xml --info
check "$info_archive"

srcml xml_error/illformedarchive_single.xml --info
#check "$info_archive"

srcml --info xml_error/illformedarchive_multi.xml
#check "$info_archive"

srcml --info xml_error/illformedarchive_single.xml
#check "$info_archive"

srcml --info < xml_error/illformedarchive_multi.xml
#check "$info_archive"

srcml --info < xml_error/illformedarchive_single.xml
#check "$info_archive"

srcml xml_error/illformedarchive_multi.xml --show-unit-count
#check "$units3" "$xml_archive_error"

srcml xml_error/illformedarchive_single.xml --show-unit-count
#check "$units1" "$xml_archive_error"

srcml --show-unit-count xml_error/illformedarchive_multi.xml
#check "$units3" "$xml_archive_error"

srcml --show-unit-count xml_error/illformedarchive_single.xml
#check "$units1" "$xml_archive_error"

srcml --show-unit-count < xml_error/illformedarchive_multi.xml
#check "$units3" "$xml_archive_error"

srcml --show-unit-count < xml_error/illformedarchive_single.xml
#check "$units1" "$xml_archive_error"

exit 0
