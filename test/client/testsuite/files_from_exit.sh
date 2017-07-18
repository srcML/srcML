#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

set +e

# file list doesn't exist
srcml --files-from nonexistent_list.txt
check_exit 1

srcml --files-from nonexistent_list.txt -o nonexistent_list.xml
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt -o nonexistent_list.xml
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.bz2.gz
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.bz2
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.cpio.bz2
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.cpio.gz
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.cpio
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.gz.bz2
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.gz
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tar.bz2
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tar.gz
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tar
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tbz2
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tgz
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.zip.bz2
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.zip.gz
check_exit 1

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.zip
check_exit 1

# file list contains nonexistent srcml file
createfile "list.txt" "nonexistent.xml"

srcml --files-from list.txt
check_exit 1


createfile "remote-list.txt" "https://github.com/srcML/test-data/blob/master/nonexistent_file.xml"

srcml --files-from remote-list.txt
check_exit 1
