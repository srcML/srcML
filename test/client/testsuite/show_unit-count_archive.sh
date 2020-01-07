#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on archive of one unit
define archive_single <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="archive_single.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

# test on archive of > 1 unit
define archive_multi <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="archive_multi.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$archive_single"
xmlcheck "$archive_multi"

define archive_single_output <<- 'STDOUT'
	1
	STDOUT

define archive_multi_output <<- 'STDOUT'
	2
	STDOUT

createfile sub/archive_single.cpp.xml "$archive_single"
createfile sub/archive_multi.cpp.xml "$archive_multi"

srcml --show-unit-count sub/archive_single.cpp.xml
check "$archive_single_output"

srcml --show-unit-count < sub/archive_single.cpp.xml
check "$archive_single_output"

srcml --show-unit-count sub/archive_multi.cpp.xml
check "$archive_multi_output"

srcml --show-unit-count < sub/archive_multi.cpp.xml
check "$archive_multi_output"

# test on on multiple input src files that create an archive
createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

srcml --show-unit-count sub/a.cpp sub/b.cpp
check "$archive_multi_output"

srcml sub/a.cpp sub/b.cpp --show-unit-count
check "$archive_multi_output"

# test count on empty archive
define empty <<- 'STDOUT'
	<unit xmlns="http://www.srcML.org/srcML/src"/>
	STDOUT

define empty_output <<- 'STDOUT'
	0
	STDOUT

xmlcheck "$empty"
createfile sub/emptyarchive.xml "$empty"

srcml --show-unit-count sub/emptyarchive.xml
check "$empty_output"

srcml --show-unit-count < sub/emptyarchive.xml
check "$empty_output"

