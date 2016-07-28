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


# input from a compressed srcml file
srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.bz2?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.gz?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.bz2.gz?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.gz.bz2?raw=true
check 3<<< "a;"


# input from an archived srcml file
srcml https://github.com/srcML/test-data/raw/master/srcml/unit.xml.cpio
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.cpio.bz2?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.cpio.gz?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.tar.bz2?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.tar.gz?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/raw/master/srcml/unit.xml.tar
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.tbz2?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.tgz?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.zip?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.zip.bz2?raw=true
check 3<<< "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/unit.xml.zip.gz?raw=true
check 3<<< "a;"
