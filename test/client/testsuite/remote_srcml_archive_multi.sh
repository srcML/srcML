#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# have to get null byte into test case result
echo -en "a;\n" > tfile
printf "\0" >> tfile
echo -en "return b;\n" >> tfile

define srca <<- 'STDOUT'
	a;
	STDOUT

# input from a valid remote srcML archive multiple units
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml
check tfile

#srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml -o sub/archive_multi.cpp
#check sub/archive_multi.cpp tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml --unit 1
check "$srca"

mkdir sub

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml --unit 1 -o sub/archive_multi.cpp
check sub/archive_multi.cpp "$srca"

# input from a compressed srcml file
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.bz2
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.gz
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.bz2.gz
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.gz.bz2
check tfile

# input from an archived srcml file
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.cpio
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.cpio.bz2
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.cpio.gz
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.tar.bz2
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.tar.gz
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.tar
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.tbz2
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.tgz
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.zip
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.zip.bz2
check tfile

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/archive-multi.xml.zip.gz
check tfile
