#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define sfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++">
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename="sub/a.cpp"/>
	INPUT

echo -n "" | src2srcml -l C++ --modifier

check 3<<< "$fsrcml"

echo -n "" | src2srcml --modifier sub/a.cpp 

check 3<<< "$fsrcml"

src2srcml -l C++ --modifier -o sub/a.cpp.xml sfile

check sub/a.cpp.xml "$srcml"

src2srcml --modifier sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml "$fsrcml"

