#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get timestamp
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" directory="sub" filename="a.cpp" timestamp="Sun Jan 11 18:39:22 2015"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"
message "timestamp provided"

srcml --show-timestamp sub/a.cpp.xml
check 3<<< "Sun Jan 11 18:39:22 2015"

srcml --show-timestamp < sub/a.cpp.xml
check 3<<< "Sun Jan 11 18:39:22 2015"


define none <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" directory="sub" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$none"
message "timestamp missing"

srcml --show-timestamp sub/a.cpp.xml
check_null

srcml --show-timestamp < sub/a.cpp.xml
check_null
