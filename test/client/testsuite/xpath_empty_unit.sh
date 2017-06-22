#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define xpath <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>

	</unit>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$xpath"
createfile sub/a.cpp.xml "$srcml"

srcml2src --xpath=/src:unit sub/a.cpp.xml
checkv2 "$xpath"

srcml2src --xpath=/src:unit < sub/a.cpp.xml
checkv2 "$xpath"

srcml2src --xpath=/src:unit -o sub/b.cpp.xml sub/a.cpp.xml
checkv2 sub/b.cpp.xml "$xpath"

srcml2src --xpath=/src:unit sub/a.cpp.xml -o sub/b.cpp.xml
checkv2 sub/b.cpp.xml "$xpath"

