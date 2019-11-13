#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"/>
	STDOUT

define xpath <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++"/>

	</unit>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$xpath"
createfile sub/a.cpp.xml "$srcml"

srcml --xpath=/src:unit sub/a.cpp.xml
check "$xpath"

srcml --xpath=/src:unit < sub/a.cpp.xml
check "$xpath"

srcml --xpath=/src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$xpath"

srcml --xpath=/src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$xpath"

