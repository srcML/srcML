#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

set +e
# test invalid_input

# unknown file
#src2srcml -l C++ foobar

#sd inputflename
#src2srcml sub/a.cpp" -o sub/a.cpp None) 2)
#wn option

echo "a;" | src2srcml -l C++ --strip

check_exit 3

# unknown encoding
echo "a;" | src2srcml -l C++ --src-encoding="ISO"

check_exit 4

echo "a;" | src2srcml -l C++ --xml-encoding="ISO"

check_exit 4

# invalid language
echo "a;" | src2srcml --language bad_language "foobar"

check_exit 6

# missing value
echo "a;" | src2srcml --language

check_exit 7

echo "a;" | src2srcml -l C++ --filename

check_exit 7

echo "a;" | src2srcml -l C++ --directory

check_exit 7

echo "a;" | src2srcml -l C++ --src-version

check_exit 7

# source encoding not given
echo "a;" | src2srcml -l C++ --src-encoding

check_exit 7

# xml encoding not given
echo "a;" | src2srcml -l C++ --xml-encoding

check_exit 7

