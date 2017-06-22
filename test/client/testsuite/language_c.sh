#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="sub/a.cpp"/>
	STDOUT

xmlcheck "$fsrcml"
createfile sub/a.cpp ""

src2srcml -l "C" sub/a.cpp
checkv2 "$fsrcml"

src2srcml --language "C" sub/a.cpp
checkv2 "$fsrcml"

src2srcml --language="C" sub/a.cpp
checkv2 "$fsrcml"

src2srcml sub/a.cpp -l "C"
checkv2 "$fsrcml"

src2srcml sub/a.cpp --language "C"
checkv2 "$fsrcml"

src2srcml sub/a.cpp --language="C"
checkv2 "$fsrcml"

src2srcml -l 'C' -o sub/a.cpp.xml sub/a.cpp
checkv2 sub/a.cpp.xml "$fsrcml"

src2srcml -l 'C' sub/a.cpp -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$fsrcml"
