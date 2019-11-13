#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# language shouldn't be on the archive unit, just on individual units
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

srcml --show-language sub/archive.cpp.xml
check "C++"

srcml --show-language < sub/archive.cpp.xml
check "C++"

# empty
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
  STDOUT

createfile sub/archive.cpp.xml "$empty"

srcml --show-language sub/archive.cpp.xml
check

srcml --show-language < sub/archive.cpp.xml
check
