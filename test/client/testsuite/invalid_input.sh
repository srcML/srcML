#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

set +e

# test invalid_input
echo "a;" | src2srcml -l C++ --strip
check_exit 3

# unknown encoding
echo "a;" | src2srcml -l C++ --src-encoding="ISO"
check_exit 4

echo "a;" | src2srcml -l C++ --xml-encoding="ISO"
check_exit 4

# source encoding not given
echo "a;" | src2srcml -l C++ --src-encoding
check_exit 7

# xml encoding not given
echo "a;" | src2srcml -l C++ --xml-encoding
check_exit 7

# invalid language
echo "a;" | src2srcml --language bad_language "foobar"
check_exit 6

# missing value
echo "a;" | src2srcml --language
check_exit 7

echo "a;" | src2srcml -l C++ --filename
check_exit 7

echo "a;" | src2srcml -l C++ --url
check_exit 7

echo "a;" | src2srcml -l C++ --src-version
check_exit 7

# invalid input filename
srcml2src foobar
check_exit 0

# unknown option
srcml2src --strip foobar
check_exit 3

# unknown encoding
srcml2src --src-encoding="ISO"
check_exit 4

# source encoding not given
srcml2src --src-encoding
check_exit 7

# unit option selected but no value
srcml2src --unit
check_exit 7

# unit value too large
srcml2src --unit 3 <<< "$nestedfile"
check_exit 1

srcml2src --unit 3 -X <<< "$nestedfile"
check_exit 1

srcml2src --unit 3 --filename <<< "$nestedfile"
check_exit 7

srcml2src --unit 3 --url <<< "$nestedfile"
check_exit 7

srcml2src --unit 3 --src-version <<< "$nestedfile"
check_exit 7

# invalid combinations
srcml2src -X --src-encoding "UTF-8" "foobar"
check_exit 0
