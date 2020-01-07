#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test hash on single unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" directory="sub" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
  STDOUT

xmlcheck "$input"
createfile sub/a.cpp.xml "$input"

message "hash provided"

srcml --show-hash sub/a.cpp.xml
check "1a2c5d67e6f651ae10b7673c53e8c502c97316d6"

srcml --show-hash < sub/a.cpp.xml
check "1a2c5d67e6f651ae10b7673c53e8c502c97316d6"

# test hash on unit with no hash provided
define none <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" directory="sub" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
  STDOUT

# test hash on archive of one unit
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
  STDOUT

# test hash on empty archive
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
  STDOUT

xmlcheck "$none"
xmlcheck "$archive"
xmlcheck "$empty"

createfile sub/a.cpp.xml "$none"
createfile sub/archive.cpp.xml "$archive"
createfile sub/emptyarchive.cpp.xml "$empty"

message "hash missing"

srcml --show-hash sub/a.cpp.xml
check

srcml --show-hash < sub/a.cpp.xml
check

srcml --show-hash sub/archive.cpp.xml
check "1a2c5d67e6f651ae10b7673c53e8c502c97316d6"

srcml --show-hash < sub/archive.cpp.xml
check "1a2c5d67e6f651ae10b7673c53e8c502c97316d6"

srcml --show-hash sub/emptyarchive.cpp.xml
check

srcml --show-hash < sub/emptyarchive.cpp.xml
check
