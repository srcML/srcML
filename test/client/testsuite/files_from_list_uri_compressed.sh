#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# file list contains an empty remote source
define empty_srcml_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile "list-empty.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp"
srcml --files-from list-empty.txt --url="test"
check "$empty_srcml_archive"
rmfile list-empty.txt

# compressed
define empty_srcml_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile "list-empty-bz2.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.bz2"
srcml --files-from list-empty-bz2.txt --url="test"
check "$empty_srcml_archive"
rmfile list-empty-bz2.txt

define empty_srcml_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile "list-empty-gz.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.gz"
srcml --files-from list-empty-gz.txt --url="test"
check "$empty_srcml_archive"
rmfile list-empty-gz.txt

define empty_srcml_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile "list-empty-bz2-gz.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.bz2.gz"
srcml --files-from list-empty-bz2-gz.txt --url="test"
check "$empty_srcml_archive"
rmfile list-empty-bz2-gz.txt

define empty_srcml_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT
createfile "list-empty-gz-b2.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.gz.bz2"
srcml --files-from list-empty-gz-b2.txt --url="test"
check "$empty_srcml_archive"
rmfile list-empty-gz-bz2.txt
