#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srca <<- 'STDOUT'a;
	return b;
	STDOUT

# input from a valid remote srcML archive multiple units
srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-multi.xml
check "$srca"

mkdir sub

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-multi.xml -o sub/archive_multi.cpp
check sub/archive_multi.cpp "a;
return b;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-multi.xml --unit 1
check "a;"

srcml https://raw.githubusercontent.com/srcML/test-data/master/srcml/archive-multi.xml --unit 1 -o sub/archive_multi.cpp
check sub/archive_multi.cpp "a;"


# input from a compressed srcml file
srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.bz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.gz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.bz2.gz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.gz.bz2?raw=true
check "a;"


# input from an archived srcml file
srcml https://github.com/srcML/test-data/raw/master/srcml/archive-multi.xml.cpio
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.cpio.bz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.cpio.gz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.tar.bz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.tar.gz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/raw/master/srcml/archive-multi.xml.tar
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.tbz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.tgz?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.zip?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.zip.bz2?raw=true
check "a;"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-multi.xml.zip.gz?raw=true
check "a;"
