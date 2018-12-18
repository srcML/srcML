#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srca <<- 'STDOUT'
	a;
	STDOUT

# input from a valid remote srcML archive with one unit
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml
check "$srca"

mkdir sub

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml -o sub/archive_single.cpp
check sub/archive_single.cpp "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml --unit 1
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml --unit 1 -o sub/archive_single.cpp
check sub/archive_single.cpp "$srca"

# input from a compressed srcml file
srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.bz2?raw=true
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.gz?raw=true
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.bz2.gz?raw=true
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.gz.bz2?raw=true
check "$srca"


# input from an archived srcml file
srcml https://github.com/srcML/test-data/raw/master/srcml/archive-single.xml.cpio
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.cpio.bz2?raw=true
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.cpio.gz?raw=true
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tar.bz2?raw=true
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tar.gz?raw=true
check "$srca"

srcml https://github.com/srcML/test-data/raw/master/srcml/archive-single.xml.tar
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tbz2?raw=true
check "$srca"

srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.tgz?raw=true
check "$srca"

# FIXME: URLs don't work
#srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip?raw=true
#check "$srca"

#srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip.bz2?raw=true
#check "$srca"

#srcml https://github.com/srcML/test-data/blob/master/srcml/archive-single.xml.zip.gz?raw=true
#check "$srca"
