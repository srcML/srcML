#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# xpath apply root test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define xpath_empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# /src:unit
srcml2src --apply-root --xpath=/src:unit sub/a.cpp.xml
check 3<<< "$output"

srcml2src --apply-root --xpath=/src:unit < sub/a.cpp.xml
check 3<<< "$output"

srcml2src --apply-root --xpath=/src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --apply-root --xpath=/src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

# //src:unit
srcml2src --apply-root --xpath=//src:unit sub/a.cpp.xml
check 3<<< "$output"

srcml2src --apply-root --xpath=//src:unit < sub/a.cpp.xml
check 3<<< "$output"

srcml2src --apply-root --xpath=//src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --apply-root --xpath=//src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

# src:unit
srcml2src --apply-root --xpath=src:unit sub/a.cpp.xml
check 3<<< "$xpath_empty"

srcml2src --apply-root --xpath=src:unit < sub/a.cpp.xml
check 3<<< "$xpath_empty"

srcml2src --apply-root --xpath=src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath_empty"

srcml2src --apply-root --xpath=src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath_empty"

