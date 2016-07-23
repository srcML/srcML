#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# input from a valid remote srcML archive with one unit
srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml
check 3<<< "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml -o sub/archive_single.cpp
check sub/archive_single.cpp 3<<< "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml --unit 1
check 3<<< "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml --unit 1 -o sub/archive_single.cpp
check sub/archive_single.cpp 3<<< "a;"
