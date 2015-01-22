#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:pos="http://www.sdml.info/srcML/position" revision="0.8.0" pos:tabs="4">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" pos:tabs="4" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/b.cpp" pos:tabs="4" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

createfile sub/a.cpp ""
createfile sub/b.cpp ""

srcml --tabs 4 --archive sub/a.cpp sub/b.cpp
check 3<<< "$fsrcml"

srcml --archive --tabs 4 sub/a.cpp sub/b.cpp
check 3<<< "$fsrcml"

srcml --tabs 4 sub/a.cpp sub/b.cpp --archive
check 3<<< "$fsrcml"

srcml sub/a.cpp sub/b.cpp --archive --tabs 4
check 3<<< "$fsrcml"

srcml --tabs 4 sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 sub/a.cpp sub/b.cpp --archive -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 4 sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --archive sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --archive
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 -o sub/a.cpp.xml --archive sub/a.cpp sub/b.cpp 
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --tabs 4 --archive -o sub/a.cpp.xml sub/a.cpp sub/b.cpp 
check sub/a.cpp.xml 3<<< "$fsrcml"

srcml --archive --tabs 4 -o sub/a.cpp.xml sub/a.cpp sub/b.cpp 
check sub/a.cpp.xml 3<<< "$fsrcml"
