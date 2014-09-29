#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test input file is gzipped

define sfile <<< "a;"

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT

define fxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp.gz">
	</unit>
	STDOUT

if platform.system() != "Windows" :

	f = open('sub/a.cpp.gz' 'r')
	gzipped = f.read()
	f.close()

	src2srcml -l C++ gzipped sxmlfile)
	src2srcml sub/a.cpp.gz' "" fxmlfile)
	if sys.platform != 'cygwin' :
	src2srcml -l C++ -o sub/a.cpp.xml gzipped
	validate(open(sub/a.cpp.xml 'r').read() sxmlfile)
	src2srcml sub/a.cpp.gz' -o sub/a.cpp.xml

	validate(open(sub/a.cpp.xml 'r').read() fxmlfile)

	

	f = open('sub/a.cpp.xml.gz' 'r')

	gzipped = f.read()
	f.close()

	srcml2src gzipped sfile)
	srcml2src 'sub/a.cpp.xml.gz' <<< "a")
	if sys.platform != 'cygwin' :
	srcml2src -o sub/a.cpp gzipped
	validate(open(sub/a.cpp 'r').read() sfile)
	srcml2src 'sub/a.cpp.xml.gz' -o sub/a.cpp ""
	validate(open(sub/a.cpp 'r').read() sfile)

##
# src2srcml Markup Extensions
