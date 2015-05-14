#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="8">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""
createfile sub/b.cpp ""

src2srcml sub/a.cpp sub/b.cpp --position --archive --in-order
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --archive --position --in-order
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --position -o sub/a.cpp.xml --archive --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --position --archive --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --position --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml --position sub/a.cpp sub/b.cpp --archive --in-order
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --archive --position --in-order
check 3<<< "$fsrcml"

src2srcml --position sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --position --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -o sub/a.cpp.xml --position sub/a.cpp sub/b.cpp --archive --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"
