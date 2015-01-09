#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" language="C++" pos:tabs="4"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" language="C++" filename="sub/a.cpp" pos:tabs="4"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --tabs '4' "" sxmlfile)
src2srcml --tabs '4' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --tabs '4' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --tabs '4' sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile


src2srcml -l C++ --tabs 1)
src2srcml -l C++ --tabs 'a' 14)
