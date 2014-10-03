#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename=sub/a.cpp/>
	INPUT

src2srcml -l C++ --operator --modifier sfile srcml)
src2srcml --operator --modifier sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --operator --modifier -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --operator --modifier sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --modifier --operator sfile srcml)

src2srcml --modifier --operator sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --modifier --operator -o sub/a.cpp.xml sfie
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --modifier --operator sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



