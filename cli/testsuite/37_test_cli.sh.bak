#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# language flg
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
	INPUT

src2srcml sub/a.cpp -l "C++"

check 3<<< "$fsrcml"

src2srcml sub/a.cpp --language "C++"

check 3<<< "$fsrcml"

src2srcml sub/a.cpp --language="C++"

check 3<<< "$fsrcml"

src2srcml -l C++ -o sub/a.cpp.xml <<< "$sfile1"

check sub/a.cpp.xml "$srcml"

src2srcml -l C++ sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml "$fsrcml"

