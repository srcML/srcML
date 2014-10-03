#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# single file test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" filename="a.cpp" language="C++">
	INPUT
</unit>
	STDOUT

xslt = STDOUTa.cpp
C++
	STDOUT

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


srcml2src --xslt + '=archive.xsl' srcml xslt)
srcml2src --xslt + '=archive.xsl' sub/a.cpp.xml xslt)
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=archive.xsl' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xslt)
srcml2src --xslt + '=archive.xsl' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xslt)


srcml2src --xslt srcml

check 4<<< "1"
srcml2src --xslt + '=' srcml

check 4<<< "1"

# archive test

