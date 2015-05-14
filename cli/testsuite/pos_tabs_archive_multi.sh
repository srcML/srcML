#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="4">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="4" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" pos:tabs="4" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""
createfile sub/b.cpp ""

srcml --tabs 4 --archive --position sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fsrcml"

srcml --archive --tabs 4 --position sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fsrcml"

srcml --tabs 4 sub/a.cpp sub/b.cpp --position --archive --in-order
check 3<<< "$fsrcml"

srcml sub/a.cpp sub/b.cpp --archive --tabs 4 --position --in-order
check 3<<< "$fsrcml"

srcml --tabs 4 --position sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --position --tabs 4 sub/a.cpp sub/b.cpp --archive -o sub/a.cpp.xml --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 4 --position sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --archive sub/a.cpp sub/b.cpp --position -o sub/a.cpp.xml --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --archive --position --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml --archive sub/a.cpp sub/b.cpp --position  --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --position --archive -o sub/a.cpp.xml sub/a.cpp sub/b.cpp  --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --position --archive --tabs 4 -o sub/a.cpp.xml sub/a.cpp sub/b.cpp  --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"
