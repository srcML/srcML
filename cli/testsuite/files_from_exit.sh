#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

set +e

# file list doesn't exist
src2srcml --files-from nonexistent_list.txt
check_exit 1

src2srcml --files-from nonexistent_list.txt -o nonexistent_list.xml
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt -o nonexistent_list.xml
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.bz2.gz
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.bz2
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.cpio.bz2
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.cpio.gz
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.cpio
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.gz.bz2
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.gz
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tar.bz2
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tar.gz
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tar
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tbz2
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.tgz
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.zip.bz2
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.zip.gz
check_exit 1

src2srcml --files-from https://raw.githubusercontent.com/srcML/test-data/master/nonexistent_list.txt.zip
check_exit 1
