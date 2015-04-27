#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test combining xpath's string function with queries
define srcml_nested <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

#define attr_out <<- 'STDOUT'
#	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
#	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION">
#
#	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" item="1" location="/src:filename[1]">b.cpp</unit>
#
#	</unit>
#	STDOUT

define attr_out <<- 'STDOUT'
	b.cpp
	STDOUT

createfile sub/a.cpp.xml "$srcml_nested"

# TODO: issue #1074
srcml2src --xpath "string(//src:unit/@filename)" sub/a.cpp.xml
check 3<<< "$attr_out"

srcml2src --xpath "string(//src:unit/@filename)" <<< "$srcml_nested"
check 3<<< "$attr_out"

	
