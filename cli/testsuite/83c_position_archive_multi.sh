#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" pos:tabs="8">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/b.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""
createfile sub/b.cpp ""

src2srcml sub/a.cpp sub/b.cpp --position --archive --ordered
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --archive --position --ordered
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --position -o sub/a.cpp.xml --archive --ordered
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --position --archive --ordered
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --position --ordered
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml --position sub/a.cpp sub/b.cpp --archive --ordered
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --archive --position --ordered
check 3<<< "$fsrcml"

src2srcml --position sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --ordered
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --position --ordered
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -o sub/a.cpp.xml --position sub/a.cpp sub/b.cpp --archive --ordered
check sub/a.cpp.xml 3<<< "$fsrcml"
