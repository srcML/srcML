#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test position
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" pos:tabs="8">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" pos:tabs="8" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

xmlcheck "$fsrcml"
createfile sub/a.cpp ""
createfile sub/b.cpp ""

srcml sub/a.cpp sub/b.cpp --position --archive
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --archive --position
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --position -o sub/a.cpp.xml --archive
check sub/a.cpp.xml "$fsrcml"

srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --position --archive
check sub/a.cpp.xml "$fsrcml"

srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --position
check sub/a.cpp.xml "$fsrcml"

srcml --position sub/a.cpp sub/b.cpp --archive
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --archive --position
check "$fsrcml"

srcml --position sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml "$fsrcml"

srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --archive --position
check sub/a.cpp.xml "$fsrcml"

srcml -o sub/a.cpp.xml --position sub/a.cpp sub/b.cpp --archive
check sub/a.cpp.xml "$fsrcml"
