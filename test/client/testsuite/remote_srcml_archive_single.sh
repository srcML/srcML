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
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.bz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.gz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.bz2.gz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.gz.bz2
check "$srca"

# input from an archived srcml file
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.cpio
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.cpio.bz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.cpio.gz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.tar.bz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.tar.gz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.tar
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.tbz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.tgz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.zip
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.zip.bz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-single.xml.zip.gz
check "$srca"
