#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define none <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" dir="sub" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$none"
message "timestamp missing"

srcml --get-timestamp sub/a.cpp.xml
check_null

srcml --get-timestamp < sub/a.cpp.xml
check_null

# TODO: issue #1042
message "timestamp provided"
exit 1
