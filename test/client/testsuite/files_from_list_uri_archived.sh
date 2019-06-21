#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define empty_srcml_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit revision="REVISION" language="C++" filename="empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile "list-empty-cpio-bz2.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.cpio.bz2"
srcml --files-from list-empty-cpio-bz2.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-cpio-gz.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.cpio.gz"
srcml --files-from list-empty-cpio-gz.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-tar-bz2.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.tar.bz2"
srcml --files-from list-empty-tar-bz2.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-tar-gz.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.tar.gz"
srcml --files-from list-empty-tar-gz.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-tbz2.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.tbz2"
srcml --files-from list-empty-tbz2.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-tgz.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.tgz"
srcml --files-from list-empty-tgz.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-zip-bz2.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.zip.bz2"
srcml --files-from list-empty-zip-bz2.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-zip-gz.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.zip.gz"
srcml --files-from list-empty-zip-gz.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-zip.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.zip"
srcml --files-from list-empty-zip.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-cpio.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.cpio"
srcml --files-from list-empty-cpio.txt --url="test"
check "$empty_srcml_archive"

createfile "list-empty-tar.txt" "https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.tar"
srcml --files-from list-empty-tar.txt --url="test"
check "$empty_srcml_archive"
