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

createfile sub/a.cpp.xml "$srcml"

# /src:unit/src:expr_stmt/src:expr/src:name
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name sub/a.cpp.xml
check 3<<< "$output1"

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name < sub/a.cpp.xml
check 3<<< "$output1"

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output1"

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output1"

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output1"

# //src:name
srcml2src --xpath=//src:name sub/a.cpp.xml
check 3<<< "$output2"

srcml2src --xpath=//src:name < sub/a.cpp.xml
check 3<<< "$output2"

srcml2src --xpath=//src:name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output2"

srcml2src --xpath=//src:name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output2"

srcml2src --xpath=//src:name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output2"

# src:name
srcml2src --xpath=src:name sub/a.cpp.xml
check 3<<< "$output_empty"

srcml2src --xpath=src:name < sub/a.cpp.xml
check 3<<< "$output_empty"

srcml2src --xpath=src:name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output_empty"

srcml2src --xpath=src:name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output_empty"

srcml2src --xpath=src:name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output_empty"
