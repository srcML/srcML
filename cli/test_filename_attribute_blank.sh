#!/bin/bash
#
# empty_language_flag_short

# test framework
source $(dirname "$0")/framework_test.sh

# setup
trap "rm -f a.cpp" EXIT

echo "" > a.cpp

# test case
srcml -l C++ --filename="foo.cpp" a.cpp

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo.cpp">
</unit>
STDOUT

# test case
srcml -l C++ a.cpp

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp">
</unit>
STDOUT
