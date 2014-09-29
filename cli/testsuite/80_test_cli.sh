#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" filename="a.cpp" language="C++"/>
	STDOUT

createfile sub/a.cpp.xml "$output"

srcml2src --xslt=archive.xsl <<< "$output"

check 3<<< "$output"

srcml2src --xslt=archive.xsl sub/a.cpp.xml

check 3<<< "$output"

srcml2src --xslt=archive.xsl -o sub/b.cpp.xml <<< "$output"

check sub/b.cpp.xml "$output"

srcml2src --xslt=archive.xsl sub/a.cpp.xml -o sub/b.cpp.xml

check sub/b.cpp.xml "$output"

srcml2src --xslt <<< "$output"

check 4<<< "1"

srcml2src --xslt= <<< "$output"

check 4<<< "1"
