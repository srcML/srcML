#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on single unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" directory="bar" filename="foo" version="1.2"/>
  STDOUT

# test on archive of one unit
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
  STDOUT

xmlcheck "$input"
xmlcheck "$archive"
createfile sub/a.cpp.xml "$input"
createfile sub/archive.cpp.xml "$archive"

srcml --show-encoding sub/a.cpp.xml
check "UTF-8"

srcml --show-encoding < sub/a.cpp.xml
check "UTF-8"

srcml --show-encoding sub/archive.cpp.xml
check "UTF-8"

srcml --show-encoding < sub/archive.cpp.xml
check "UTF-8"

# test on empty archive
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
  <unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
  STDOUT

createfile sub/emptyarchive.xml "$empty"

srcml --show-encoding sub/emptyarchive.xml
check "UTF-8"

srcml --show-encoding < sub/emptyarchive.xml
check "UTF-8"
