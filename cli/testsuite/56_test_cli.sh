#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:type="http://www.sdml.info/srcML/modifier" revision="0.8.0" language="C++"/>
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:type="http://www.sdml.info/srcML/modifier" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	INPUT

src2srcml -l C++ --literal --modifier sfile srcml)
src2srcml --literal --modifier sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --literal --modifier -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --literal --modifier sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --modifier --literal sfile srcml)

src2srcml --modifier --literal sub/a.cpp
if sys.platform != 'cygwin' :
	src2srcml -l C++ --modifier --literal -o sub/a.cpp.xml sfie
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --modifier --literal sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



