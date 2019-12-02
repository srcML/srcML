#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get directory
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$archive"
createfile sub/archive.cpp.xml "$archive"

srcml --show-timestamp sub/archive.cpp.xml
check

srcml --show-timestamp < sub/archive.cpp.xml
check

# empty
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$empty"
createfile sub/archive.cpp.xml "$empty"

srcml --show-timestamp sub/archive.cpp.xml
check

srcml --show-timestamp < sub/archive.cpp.xml
check

# none
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDIN

xmlcheck "$none"
createfile sub/archive.cpp.xml "$none"

srcml --show-timestamp sub/archive.cpp.xml
check

srcml --show-timestamp < sub/archive.cpp.xml
check

