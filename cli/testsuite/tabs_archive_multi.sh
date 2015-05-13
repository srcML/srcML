#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="8">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""
createfile sub/b.cpp ""

# specify tab size of 8
srcml --tabs 8 --archive sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fsrcml"

srcml --archive --tabs 8 sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fsrcml"

srcml --tabs 8 sub/a.cpp sub/b.cpp --archive --in-order
check 3<<< "$fsrcml"

srcml sub/a.cpp sub/b.cpp --archive --tabs 8 --in-order
check 3<<< "$fsrcml"

srcml --tabs 8 sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 sub/a.cpp sub/b.cpp --archive -o sub/a.cpp.xml --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 8 sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 --archive sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --archive --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 -o sub/a.cpp.xml --archive sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 8 --archive -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 8 -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"


# use default tab size of 8
srcml --tabs --archive sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fsrcml"

srcml sub/a.cpp sub/b.cpp --archive --tabs --in-order
check 3<<< "$fsrcml"

srcml --tabs --archive sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --archive --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs -o sub/a.cpp.xml --archive sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs --archive -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"