#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" language="C++" pos:tabs="8"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --position "" sxmlfile)
src2srcml --position sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --position -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --position sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile
