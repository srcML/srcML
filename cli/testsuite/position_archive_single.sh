#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="8">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="8">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""

src2srcml -l C++ --position --archive < sub/a.cpp
check 3<<< "$srcml"

src2srcml -l C++ --archive --position < sub/a.cpp
check 3<<< "$srcml"

src2srcml -l C++ --position  --archive -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml -l C++ --archive --position -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --position -l C++ --archive -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml -l C++ --archive -o sub/a.cpp.xml --position < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml sub/a.cpp --position --archive
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --archive --position
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --position -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp -o sub/a.cpp.xml --position --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp -o sub/a.cpp.xml --archive --position
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml --position sub/a.cpp --archive
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --archive --position
check 3<<< "$fsrcml"

src2srcml --position sub/a.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp -o sub/a.cpp.xml --archive --position
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -o sub/a.cpp.xml --position sub/a.cpp --archive
check sub/a.cpp.xml 3<<< "$fsrcml"
