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


# file list of non-existent files
define open_error <<- 'STDERR'
	srcml: Unable to open file nonexistent1.txt
	srcml: Unable to open file nonexistent2.txt
STDERR

createfile nonexistent_files.txt "nonexistent1.txt
nonexistent2.txt"

src2srcml --files-from nonexistent_files.txt --in-order
check 3<<< "$empty_srcml" 4<<< "$open_error"

src2srcml --files-from nonexistent_files.txt --in-order -o nonexistent.xml
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
