#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="sub/a.cpp"/>
	STDOUT

xmlcheck "$fsrcml"
createfile sub/a.cpp ""

srcml -l "C" sub/a.cpp
check "$fsrcml"

srcml --language "C" sub/a.cpp
check "$fsrcml"

srcml --language="C" sub/a.cpp
check "$fsrcml"

srcml sub/a.cpp -l "C"
check "$fsrcml"

srcml sub/a.cpp --language "C"
check "$fsrcml"

srcml sub/a.cpp --language="C"
check "$fsrcml"

srcml -l 'C' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -l 'C' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"
