#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# srcML info and longinfo

define info <<- 'STDOUT'
	xmlns=http://www.srcML.org/srcML/src xmlns:cpp=http://www.srcML.org/srcML/cpp
	encoding="UTF-8"
	language="C++"
	url="test"
	filename="sub/a.cpp"
	STDOUT

define longinfo <<- 'STDOUT'
	xmlns=http://www.srcML.org/srcML/src xmlns:cpp=http://www.srcML.org/srcML/cpp
	encoding="UTF-8"
	language="C++"
	url="test"
	filename="sub/a.cpp"
	1
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/archive_single.xml "$srcml"

srcml sub/archive_single.xml -i
check 3<<< "$info"

srcml sub/archive_single.xml --info
check 3<<< "$info"

srcml --info < sub/archive_single.xml
check 3<<< "$info"

srcml -i sub/archive_single.xml
check 3<<< "$info"

srcml --info sub/archive_single.xml
check 3<<< "$info"

srcml --longinfo sub/archive_single.xml
check 3<<< "$longinfo"

srcml sub/archive_single.xml --longinfo
check 3<<< "$longinfo"

srcml --longinfo < sub/archive_single.xml
check 3<<< "$longinfo"
