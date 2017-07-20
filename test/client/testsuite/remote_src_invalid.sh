#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# input from an invalid remote source file
srcml https://nonexistent-file.cpp
check_exit 1

srcml https://nonexistent-file.cpp -o remote.xml
check_exit 1

srcml https://nonexistent-file.cpp --archive
check_exit 1

srcml https://nonexistent-file.cpp	--archive -o remote_archive.xml
check_exit 1

# files from contains an invalid remote source file
createfile list.txt "https://nonexistent-file.cpp"

srcml --files-from list.txt
check_exit 1

srcml --files-from list.txt -o remote_from_list.xml
check_exit 1
