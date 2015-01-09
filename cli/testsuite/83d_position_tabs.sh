#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position and tabs

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" language="C++" pos:tabs="2"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" language="C++" filename=sub/a.cpp pos:tabs="2"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --position --tabs '2' "" sxmlfile)
src2srcml --position --tabs '2' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --position --tabs '2' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --position --tabs '2' sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile


src2srcml -l C++ --position --tabs 1)
src2srcml -l C++ --position --tabs 'a' 14)

src2srcml -l C++ --tabs '2' --position "" sxmlfile)
src2srcml --tabs '2' --position sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --tabs '2' --position -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --tabs '2' --position sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --tabs --position 14)
src2srcml -l C++ --tabs 'a' --position 14)
