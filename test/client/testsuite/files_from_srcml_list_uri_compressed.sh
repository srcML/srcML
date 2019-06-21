#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# file list contains an empty remote source
define empty_srcml_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit revision="REVISION" language="C++" filename="empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile list-empty.txt "https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/emptyunit.xml"

srcml --files-from list-empty.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-bz2.txt "https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/emptyunit.xml.bz2"

srcml --files-from list-empty-bz2.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-gz.txt "https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/emptyunit.xml.gz"

srcml --files-from list-empty-gz.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-bz2-gz.txt "https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/emptyunit.xml.bz2.gz"

srcml --files-from list-empty-bz2-gz.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-gz-b2.txt "https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/emptyunit.xml.gz.bz2"

srcml --files-from list-empty-gz-b2.txt --url="test"
check "$empty_srcml_archive"
