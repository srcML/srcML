#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION">

	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/src" xmlns:bar="http://www.cs.uakron.edu/~collard/bar" revision="REVISION" language="Java">
	<cpp:expr_stmt><cpp:expr><cpp:name>b</cpp:name></cpp:expr>;</cpp:expr_stmt>
	</unit>

	</unit>
	STDOUT

#	<unit revision="REVISION" item="1" location="/src:expr_stmt[1]/src:expr[1]/src:name[1]"><name>a</name></unit>

#	<unit revision="REVISION" language="Java" item="1" location="/cpp:expr_stmt[1]/cpp:expr[1]/cpp:name[1]"><cpp:name>b</cpp:name></unit>
define xpath <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.sdml.info/srcML/src" revision="REVISION" language="C++" item="1"><name>a</name></unit>

	<unit revision="REVISION" language="Java" item="1"><cpp:name>b</cpp:name></unit>

	</unit>
	STDOUT

#	<unit revision="REVISION" item="1" location="/src:expr_stmt[1]/src:expr[1]/src:name[1]"><name>a</name></unit>

#	<unit revision="REVISION" language="Java" item="1" location="/cpp:expr_stmt[1]/cpp:expr[1]/cpp:name[1]"><cpp:name>b</cpp:name></unit>
define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.sdml.info/srcML/src" revision="REVISION" language="C++" item="1"><name>a</name></unit>

	<unit revision="REVISION" language="Java" item="1"><cpp:name>b</cpp:name></unit>

	</unit>
	STDOUT

define output_empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION"/>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# /src:unit/src:expr_stmt/src:expr/src:name
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name sub/a.cpp.xml
check 3<<< "$xpath"

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name < sub/a.cpp.xml
check 3<<< "$xpath"

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath"

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath"

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$xpath"

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
