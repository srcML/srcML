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

src2srcml --files-from empty.txt
check 3<<< "$empty_srcml"

src2srcml --files-from empty.txt -o empty.xml
check empty.xml 3<<< "$empty_srcml"

# empty remote source input
src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/empty/empty-file-list.txt
check 3<<< "$empty_srcml"

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/empty/empty-file-list.txt -o empty-remote.xml
check empty-remote.xml 3<<< "$empty_srcml"


# file list of non-existent files
define open_error <<- 'STDERR'
	srcml: Unable to open file nonexistent1.txt
STDERR

createfile nonexistent_files.txt "nonexistent1.txt"

src2srcml --files-from nonexistent_files.txt
check 3<<< "$empty_srcml" 4<<< "$open_error"

src2srcml --files-from nonexistent_files.txt -o nonexistent.xml
check nonexistent.xml 3<<< "$empty_srcml" 4<<< "$open_error"


# file list references itself
define open_error <<- 'STDOUT'
	srcml: Unable to open file loop.txt
	STDOUT

createfile loop.txt "loop.txt"

src2srcml --files-from loop.txt
check 4<<< "$open_error" 3<<< "$empty_srcml"

src2srcml --files-from loop.txt -o loop.xml
check 4<<< "$open_error" loop.xml 3<<< "$empty_srcml"


# file list references empty file
define empty_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="empty.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile empty.cpp ""
createfile filelist.txt "empty.cpp"

src2srcml --files-from filelist.txt
check 3<<< "$empty_archive"

src2srcml --files-from filelist.txt -o files-from-empty-cpp.xml
check files-from-empty-cpp.xml 3<<< "$empty_archive"

src2srcml --files-from filelist.txt --archive
check 3<<< "$empty_archive"

src2srcml --files-from filelist.txt --archive -o files-from-empty-cpp.xml
check files-from-empty-cpp.xml 3<<< "$empty_archive"


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

src2srcml --files-from empty.txt.cpio --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.cpio.bz2 --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.cpio.gz --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.tar.bz2 --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.tar.gz --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.tar --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.tbz2 --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.tgz --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.zip --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.zip.bz2 --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from empty.txt.zip.gz --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

rmfile empty.txt
rmfile empty.txt.cpio
rmfile empty.txt.cpio.bz2
rmfile empty.txt.cpio.gz
rmfile empty.txt.tar.bz2
rmfile empty.txt.tar.gz
rmfile empty.txt.tar
rmfile empty.txt.tbz2
rmfile empty.txt.tgz
rmfile empty.txt.zip
rmfile empty.txt.zip.bz2
rmfile empty.txt.zip.gz


# empty file list from remote archived/crompressed file
src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/raw/master/empty/empty.txt.cpio --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.cpio.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.cpio.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.tar.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.tar.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/raw/master/empty/empty.txt.tar --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.tbz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.tgz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.zip?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.zip.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"

src2srcml --files-from https://github.com/srcML/test-data/blob/master/empty/empty.txt.zip.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url" 4<<< "srcml: files-from expecting non-archived file"
