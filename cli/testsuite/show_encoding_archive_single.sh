#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get directory
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/archive.cpp.xml "$archive"

srcml --show-encoding sub/archive.cpp.xml
check 3<<< "UTF-8"

srcml --show-encoding < sub/archive.cpp.xml
check 3<<< "UTF-8"

define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0"/>
	STDOUT

createfile sub/emptyarchive.xml "$empty"

srcml --show-encoding sub/emptyarchive.xml
check 3<<< "UTF-8"

srcml --show-encoding < sub/emptyarchive.xml
check 3<<< "UTF-8"
