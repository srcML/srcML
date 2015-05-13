#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# files from
define empty_srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
	STDOUT

# empty file input
createfile empty.txt ""

src2srcml --files-from empty.txt
check 3<<< "$empty_srcml"

src2srcml --files-from empty.txt -o empty.xml
check empty.xml 3<<< "$empty_srcml"


# file list of non-existent files
createfile nonexistent_files.txt "nonexistent1.txt
nonexistent2.txt"

src2srcml --files-from nonexistent_files
check 3<<< "$empty_srcml"

src2srcml --files-from nonexistent_files -o nonexistent.xml
check nonexistent.xml 3<<< "$empty_srcml"


# file list references itself
define open_error <<- 'STDOUT'
	srcml: Unable to open file loop.txt
	STDOUT

createfile loop.txt "loop.txt"

src2srcml --files-from loop.txt
check 4<<< "$open_error" 3<<< "$empty_srcml"

src2srcml --files-from loop.txt -o loop.xml
check 4<<< "$open_error" loop.xml 3<<< "$empty_srcml"


# file list doesn't exist

src2srcml --files-from nonexistant_list.txt
check 3<<< "$empty_srcml"

src2srcml --files-from nonexistant_list.txt -o nonexistant_list.xml
check nonexistant_list.xml 3<<< "$empty_srcml"
