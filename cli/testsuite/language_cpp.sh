#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# language flg
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

src2srcml -l "C++" sub/a.cpp
check 3<<< "$fsrcml"

src2srcml --language "C++" sub/a.cpp
check 3<<< "$fsrcml"

src2srcml --language="C++" sub/a.cpp
check 3<<< "$fsrcml"

src2srcml sub/a.cpp -l "C++"
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --language "C++"
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --language="C++"
check 3<<< "$fsrcml"

src2srcml -l C++ -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml -l C++ sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"
