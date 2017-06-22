#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# input from a valid remote srcML archive with one unit
srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml
check "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml -o sub/archive_single.cpp
check sub/archive_single.cpp "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml --unit 1
check "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml --unit 1 -o sub/archive_single.cpp
check sub/archive_single.cpp "a;"

# input from a compressed srcml file
srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.bz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.gz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.bz2.gz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.gz.bz2?raw=true
check "a;"


# input from an archived srcml file
srcml https://github.com/srcML/test-data/raw/master/srcml/archive-single.xml.cpio
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.cpio.bz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.cpio.gz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tar.bz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tar.gz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/raw/master/srcml/archive-single.xml.tar
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tbz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tgz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip.bz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip.gz?raw=true
check "a;"
