#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
	<unit language="Java">
	<foo:a/>
	</unit>

	<unit xmlns:bar="http://www.cs.uakron.edu/~collard/foo" revision="0.8.0" language="Java">
	<foo:a/><bar:b/>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit revision="0.8.0" language="Java">
	<foo:a/>
	</unit>

	<unit revision="0.8.0" language="Java">
	<foo:a/><bar:b/>
	</unit>

	</unit>
	STDOUT

define xpath_empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo"/>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# /src:unit
srcml2src --xpath=/src:unit sub/a.cpp.xml
check 3<<< "$output"

srcml2src --xpath=/src:unit < sub/a.cpp.xml
check 3<<< "$output"

srcml2src --xpath=/src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --xpath=/src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --xpath=/src:unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

# //src:unit
srcml2src --xpath=//src:unit sub/a.cpp.xml
check 3<<< "$output"

srcml2src --xpath=//src:unit < sub/a.cpp.xml
check 3<<< "$output"

srcml2src --xpath=//src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --xpath=//src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --xpath=//src:unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

# src:unit
srcml2src --xpath=src:unit sub/a.cpp.xml
check 3<<< "$xpath_empty"

srcml2src --xpath=src:unit < sub/a.cpp.xml
check 3<<< "$xpath_empty"

srcml2src --xpath=src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath_empty"

srcml2src --xpath=src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath_empty"

srcml2src --xpath=src:unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath_empty"

