#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define error <<- 'STDOUT'
	srcml: Unable to open file https://nonexistent-file.cpp
	STDOUT

# input from an invalid remote source file
srcml https://nonexistent-file.cpp
check 4<<< "$error"

srcml https://nonexistent-file.cpp -o remote.xml
check 4<<< "$error"

srcml https://nonexistent-file.cpp --archive
ccheck 4<<< "$error"

srcml https://nonexistent-file.cpp	--archive -o remote_archive.xml
check 4<<< "$error"


# files from contains an invalid remote source file
createfile list.txt "https://nonexistent-file.cpp"

srcml --files-from list.txt
check 4<<< "$error"

srcml --files-from list.txt -o remote_from_list.xml
check 4<<< "$error"
