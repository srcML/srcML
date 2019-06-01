#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
#	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" item="1" location="/src:expr_stmt[1]/src:expr[1]/src:name[1]"><name>a</name></unit>
define output1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" item="1"><name>a</name></unit>

	</unit>
	STDOUT

#	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" item="1" location="/src:expr_stmt[1]/src:expr[1]/src:name[1]"><name>a</name></unit>
define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" item="1"><name>a</name></unit>

	</unit>
	STDOUT

define output_empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$output1"
xmlcheck "$output2"
xmlcheck "$output_empty"
xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

# /src:unit/src:expr_stmt/src:expr/src:name
srcml --xpath=/src:unit/src:expr_stmt/src:expr/src:name sub/a.cpp.xml
check "$output1"

srcml --xpath=/src:unit/src:expr_stmt/src:expr/src:name sub/a.cpp.xml --output-src
check "a"

srcml --xpath=/src:unit/src:expr_stmt/src:expr/src:name < sub/a.cpp.xml
check "$output1"

srcml --xpath=/src:unit/src:expr_stmt/src:expr/src:name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath=/src:unit/src:expr_stmt/src:expr/src:name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath=/src:unit/src:expr_stmt/src:expr/src:name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output1"

# //src:name
srcml --xpath=//src:name sub/a.cpp.xml
check "$output2"

srcml --xpath=//src:name < sub/a.cpp.xml
check "$output2"

srcml --xpath=//src:name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output2"

srcml --xpath=//src:name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output2"

srcml --xpath=//src:name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output2"

 # src:name
srcml --xpath=src:name sub/a.cpp.xml
check "$output2"

srcml --xpath=src:name < sub/a.cpp.xml
check "$output2"

srcml --xpath=src:name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output2"

srcml --xpath=src:name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output2"

srcml --xpath=src:name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output2"

# src:name
srcml --xpath=name sub/a.cpp.xml
check "$output_empty"

srcml --xpath=name < sub/a.cpp.xml
check "$output_empty"

srcml --xpath=name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output_empty"

srcml --xpath=name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output_empty"

srcml --xpath=name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output_empty"
