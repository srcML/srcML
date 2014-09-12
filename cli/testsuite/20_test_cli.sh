#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test invalid_input

# unknown file
#src2srcml -l C++ foobar

#sd inputflename
#src2srcml sub/a.cpp" -o sub/a.cpp None) 2)
#wn option

src2srcml -l C++ --strip << "a;"

check 4<< "3"

# unknown encoding
src2srcml -l C++ --src-encoding="ISO" <<< "a;"

check 4<< "4"

src2srcml -l C++ --encoding="ISO" <<< "a;"

check 4<< "4"

# invalid language
src2srcml --language bad_language "foobar"

check 4<< "6"

# missing value
src2srcml --language

check 4<< "7"

src2srcml -l C++ --filename 8

check 4<< "8"

src2srcml -l C++ --directory 9

check 4<< "9"

src2srcml -l C++ --src-version 9

check 4<< "9"

# source encoding not given
src2srcml -l C++ --src-encoding

check 4<< "11"

# xml encoding not given
src2srcml -l C++ --encoding

check 4<< "12"

