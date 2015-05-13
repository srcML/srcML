#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test expression_mode

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><macro><name>a</name></macro></unit>
	STDOUT

define fsxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><macro><name>a</name></macro></unit>
	STDOUT

createfile sub/a.cpp "a"

src2srcml -e sub/a.cpp

check 3<<< "$fsxmlfile"

src2srcml --expression sub/a.cpp

check 3<<< "$fsxmlfile"

src2srcml sub/a.cpp -e

check 3<<< "$fsxmlfile"

src2srcml sub/a.cpp --expression

check 3<<< "$fsxmlfile"

src2srcml -l C++ --expression -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml --expression sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsxmlfile"
