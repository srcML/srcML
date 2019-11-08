#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test output options
define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define axml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define bxml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$xmlfile"
xmlcheck "$axml"
xmlcheck "$bxml"
createfile sub/a.xml "$axml"
createfile sub/b.xml "$bxml"

# issue #1368
srcml sub/a.xml sub/b.xml output.tar
check_exit 1

# issue 1367
srcml sub/a.xml filedoesntexist.xml output.tar
check_exit 1
