#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# input from a valid remote srcML archive with one unit
srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml
checkv2 "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml -o sub/archive_single.cpp
checkv2 sub/archive_single.cpp "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml --unit 1
checkv2 "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-single.xml --unit 1 -o sub/archive_single.cpp
checkv2 sub/archive_single.cpp "a;"

# input from a compressed srcml file
srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.bz2?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.gz?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.bz2.gz?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.gz.bz2?raw=true
checkv2 "a;"


# input from an archived srcml file
srcml https://github.com/srcML/test-data/raw/master/srcml/archive-single.xml.cpio
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.cpio.bz2?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.cpio.gz?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tar.bz2?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tar.gz?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/raw/master/srcml/archive-single.xml.tar
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tbz2?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tgz?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip.bz2?raw=true
checkv2 "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip.gz?raw=true
checkv2 "a;"
