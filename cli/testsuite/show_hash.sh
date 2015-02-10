#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get hash
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C" directory="sub" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"
message "hash provided"

# TODO: issue #1041
srcml --show-hash sub/a.cpp.xml
check 3<<< "1a2c5d67e6f651ae10b7673c53e8c502c97316d6"

srcml --show-hash < sub/a.cpp.xml
check 3<<< "1a2c5d67e6f651ae10b7673c53e8c502c97316d6"


define none <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C" directory="sub" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$none"
message "hash missing"

srcml --show-hash sub/a.cpp.xml
check_null

srcml --show-hash < sub/a.cpp.xml
check_null
