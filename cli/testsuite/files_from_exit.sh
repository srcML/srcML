#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

set +e

# file list doesn't exist
src2srcml --files-from nonexistent_list.txt
check_exit 1

src2srcml --files-from nonexistent_list.txt -o nonexistent_list.xml
check_exit 1
