#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# input from a valid remote srcML archive multiple units
srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-multi.xml
check 3<<< "a;
return b;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-multi.xml -o sub/archive_multi.cpp
check sub/archive_multi.cpp 3<<< "a;
return b;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-multi.xml --unit 1
check 3<<< "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-multi.xml --unit 1 -o sub/archive_multi.cpp
check sub/archive_multi.cpp 3<<< "a;"
