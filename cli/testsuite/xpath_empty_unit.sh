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

createfile sub/a.cpp.xml "$srcml"

srcml2src --xpath=/src:unit sub/a.cpp.xml
check 3<<< "$xpath"

srcml2src --xpath=/src:unit < sub/a.cpp.xml
check 3<<< "$xpath"

srcml2src --xpath=/src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath"

srcml2src --xpath=/src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath"

