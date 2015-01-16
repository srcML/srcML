#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get hash
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" dir="sub" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"
message "hash provided"

srcml --get-hash sub/a.cpp.xml
check 3<<< "1a2c5d67e6f651ae10b7673c53e8c502c97316d6"

srcml --get-hash < sub/a.cpp.xml
check 3<<< "1a2c5d67e6f651ae10b7673c53e8c502c97316d6"


define none <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" dir="sub" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$none"
message "hash missing"

srcml --get-hash sub/a.cpp.xml
check_null

srcml --get-hash < sub/a.cpp.xml
check_null
