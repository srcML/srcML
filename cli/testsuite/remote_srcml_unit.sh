#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# input from a valid remote srcml file
srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/unit.xml
check 3<<< "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/unit.xml -o sub/unit.cpp
check sub/unit.cpp 3<<< "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/unit.xml --unit 1
check 3<<< "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/unit.xml --unit 1 -o sub/unit.cpp
check sub/unit.cpp 3<<< "a;"
