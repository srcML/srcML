#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# files from
define empty_srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
	STDOUT
xmlcheck "$empty_srcml"

# empty file input
createfile empty.txt " "

srcml --files-from empty.txt
check "$empty_srcml"

srcml --files-from empty.txt -o empty.xml
check empty.xml "$empty_srcml"

# empty remote source input
srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty-file-list.txt
check "$empty_srcml"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty-file-list.txt -o empty-remote.xml
check empty-remote.xml "$empty_srcml"

# file list of non-existent files
define open_error <<- 'STDERR'
	srcml: Extension not supported nonexistent1.txt
	STDERR

createfile nonexistent_files.txt "nonexistent1.txt"

srcml --files-from nonexistent_files.txt
check_exit 1 "$empty_srcml" "$open_error"

srcml --files-from nonexistent_files.txt -o nonexistent.xml
check_exit 1 "$open_error"

# file list references itself
define open_error <<- 'STDOUT'
	srcml: Extension not supported loop.txt
	STDOUT

createfile loop.txt "loop.txt"

srcml --files-from loop.txt
check_exit 1 "$empty_srcml" "$open_error"

srcml --files-from loop.txt -o loop.xml
check_exit 1 "$open_error"
#loop.xml "$empty_srcml"

# file list references empty file
define empty_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile empty.cpp ""
createfile filelist.txt "empty.cpp"

srcml --files-from filelist.txt
check "$empty_archive"

srcml --files-from filelist.txt -o files-from-empty-cpp.xml
check files-from-empty-cpp.xml "$empty_archive"

srcml --files-from filelist.txt --archive
check "$empty_archive"

srcml --files-from filelist.txt --archive -o files-from-empty-cpp.xml
check files-from-empty-cpp.xml "$empty_archive"

# empty archived file list
define empty_srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test"/>
	STDOUT

echo empty.txt | tr " " "\n" | cpio --quiet -o > empty.txt.cpio
bzip2 -c empty.txt.cpio > empty.txt.cpio.bz2
gzip -c empty.txt.cpio > empty.txt.cpio.gz
tar -cjf empty.txt.tar.bz2 empty.txt
tar -czf empty.txt.tar.gz empty.txt
tar -cf empty.txt.tar empty.txt
tar -cjf empty.txt.tbz2 empty.txt
tar -czf empty.txt.tgz empty.txt
zip --quiet empty.txt.zip empty.txt
bzip2 -c empty.txt.zip > empty.txt.zip.bz2
gzip -c empty.txt.zip > empty.txt.zip.gz

srcml --files-from empty.txt.cpio --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.cpio.bz2 --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.cpio.gz --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.tar.bz2 --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.tar.gz --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.tar --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.tbz2 --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.tgz --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.zip --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.zip.bz2 --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from empty.txt.zip.gz --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

# empty file list from remote archived/crompressed file
srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.bz2 --url="test"
check "$empty_srcml_with_url"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.cpio --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.cpio.bz2 --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.cpio.gz --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.tar.bz2 --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.tar.gz --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.tar --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.tbz2 --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.tgz --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.zip --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.zip.bz2 --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.zip.gz --url="test"
check "$empty_srcml_with_url" "srcml: filelist requires a non-archived file format\n"
