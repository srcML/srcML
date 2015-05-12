#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# language shouldn't be on the archive unit, just on individual units
# none
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDIN

createfile sub/archive.cpp.xml "$none"

srcml --show-language sub/archive.cpp.xml
check 3<<< "C++"

srcml --show-language < sub/archive.cpp.xml
check 3<<< "C++"

# empty
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0"/>
	STDOUT

createfile sub/archive.cpp.xml "$empty"

srcml --show-language sub/archive.cpp.xml
check_null

srcml --show-language < sub/archive.cpp.xml
check_null
