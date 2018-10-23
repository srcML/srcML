#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srca <<- 'STDOUT'
	a;
	STDOUT

# input from a valid remote srcml file
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml
check "$srca"

mkdir sub

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml -o sub/unit.cpp
check sub/unit.cpp "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml --unit 1
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml --unit 1 -o sub/unit.cpp
check sub/unit.cpp "$srca"

# input from a compressed srcml file
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.bz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.gz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.bz2.gz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.gz.bz2
check "$srca"

# input from an archived srcml file
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.cpio
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.cpio.bz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.cpio.gz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.tar.bz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.tar.gz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.tar
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.tbz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.tgz
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.zip
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.zip.bz2
check "$srca"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/srcml/unit.xml.zip.gz
check "$srca"
