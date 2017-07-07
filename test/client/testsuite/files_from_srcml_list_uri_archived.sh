#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define empty_srcml_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile list-empty-cpio-bz2.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.cpio.bz2"

src2srcml --files-from list-empty-cpio-bz2.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-cpio-gz.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.cpio.gz"

src2srcml --files-from list-empty-cpio-gz.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-tar-bz2.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.tar.bz2"

src2srcml --files-from list-empty-tar-bz2.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-tar-gz.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.tar.gz"

src2srcml --files-from list-empty-tar-gz.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-tbz2.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.tbz2"

src2srcml --files-from list-empty-tbz2.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-tgz.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.tgz"

src2srcml --files-from list-empty-tgz.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-zip-bz2.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.zip.bz2"

src2srcml --files-from list-empty-zip-bz2.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-zip-gz.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.zip.gz"

src2srcml --files-from list-empty-zip-gz.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-zip.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.zip"

src2srcml --files-from list-empty-zip.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-cpio.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.cpio"

src2srcml --files-from list-empty-cpio.txt --url="test"
check "$empty_srcml_archive"

createfile list-empty-tar.txt "https://raw.githubusercontent.com/srcML/test-data/master/srcml/emptyunit.xml.tar"

src2srcml --files-from list-empty-tar.txt --url="test"
check "$empty_srcml_archive"
