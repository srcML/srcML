#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp=	STDOUT

define output <<- 'STDOUT'
	f<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp=""	STDOUT

src2srcml -l C++ --literal --operator --modifier sfile srcml)
src2srcml --literal --operator --modifier sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --literal --operator --modifier -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --literal --operator --modifier sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --literal --modifier --operator sfile srcml)

src2srcml --literal --modifier --operator sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --literal --modifier --operator -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --literal --modifier --operator sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --operator --literal --modifier sfile srcml)

src2srcml --operator --literal --modifier sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --operator --literal --modifier -o sub/a.cpp.xml sfile
	 validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --operator --literal --modifier sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --operator --modifier --literal sfile srcml)

src2srcml --operator --modifier --literal sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --operator --modifier --literal -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --operator --modifier --literal sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --modifier --literal --operator sfile srcml)

src2srcml --modifier --literal --operator sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --modifier --literal --operator -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --modifier --literal --operator sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --modifier --operator --literal sfile srcml)

src2srcml --modifier --operator --literal sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --modifier --operator --literal -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --modifier --operator --literal sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



