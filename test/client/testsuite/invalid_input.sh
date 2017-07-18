#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

set +e

# test invalid_input
echo "a;" | srcml -l C++ --strip
check_exit 3

# unknown encoding
echo "a;" | srcml -l C++ --src-encoding="ISO"
check_exit 4

echo "a;" | srcml -l C++ --xml-encoding="ISO"
check_exit 4

# source encoding not given
echo "a;" | srcml -l C++ --src-encoding
check_exit 7

# xml encoding not given
echo "a;" | srcml -l C++ --xml-encoding
check_exit 7

# invalid language
echo "a;" | srcml --language bad_language "foobar"
check_exit 6

# missing value
echo "a;" | srcml --language
check_exit 7

echo "a;" | srcml -l C++ --filename
check_exit 7

echo "a;" | srcml -l C++ --url
check_exit 7

echo "a;" | srcml -l C++ --src-version
check_exit 7

# invalid input filename
srcml foobar
check_exit 1

# unknown option
srcml --strip foobar
check_exit 3

# unknown encoding
srcml --src-encoding="ISO"
check_exit 4

# source encoding not given
srcml --src-encoding
check_exit 7

# unit option selected but no value
srcml --unit
check_exit 7

# unit value too large
srcml --unit 3 <<< "$nestedfile"
check_exit 1

srcml --unit 3 -X <<< "$nestedfile"
check_exit 1

srcml --unit 3 --filename <<< "$nestedfile"
check_exit 7

srcml --unit 3 --url <<< "$nestedfile"
check_exit 7

srcml --unit 3 --src-version <<< "$nestedfile"
check_exit 7

# invalid combinations
srcml -X --src-encoding "UTF-8" "foobar"
check_exit 1
