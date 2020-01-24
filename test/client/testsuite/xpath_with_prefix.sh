#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test other_prefixes

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo" xmlns:bar="http://www.cs.uakron.edu/~collard/bar" >

	<unit revision="REVISION" language="Java">
	<foo:a/>
	</unit>

	<unit revision="REVISION" language="Java">
	<bar:b/>
	</unit>

	</unit>
	STDOUT

define xpathempty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo" xmlns:bar="http://www.cs.uakron.edu/~collard/bar" revision="REVISION"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo" xmlns:bar="http://www.cs.uakron.edu/~collard/bar" revision="REVISION">

	<unit revision="REVISION" language="Java">
	<foo:a/>
	</unit>

	<unit revision="REVISION" language="Java">
	<bar:b/>
	</unit>

	</unit>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$xpathempty"
xmlcheck "$output"

createfile sub/a.cpp.xml "$srcml"

# /src:unit
srcml --xpath="/src:unit" sub/a.cpp.xml
check "$output"

srcml --xpath="/src:unit" < sub/a.cpp.xml
check "$output"

srcml --xpath="/src:unit" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output"

srcml --xpath="/src:unit" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output"

srcml --xpath="/src:unit" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output"

# //src:unit
srcml --xpath="//src:unit" sub/a.cpp.xml
check "$output"

srcml --xpath="//src:unit" < sub/a.cpp.xml
check "$output"

srcml --xpath="//src:unit" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output"

srcml --xpath="//src:unit" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output"

srcml --xpath="//src:unit" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output"

# unit
srcml --xpath=unit sub/a.cpp.xml
check "$xpathempty"

srcml --xpath=unit < sub/a.cpp.xml
check "$xpathempty"

srcml --xpath=unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$xpathempty"

srcml --xpath=unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$xpathempty"

srcml --xpath=unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$xpathempty"

