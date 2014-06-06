#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define nestedfile <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDIN

define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	</unit>
	STDOUT

srcml2src --xml --unit "1" - <<< "$nestedfile"

check 3<<< "$sxmlfile1"

srcml2src --xml --unit "1" <<< "$nestedfile"

check 3<<< "$sxmlfile1"

define sxmlfile2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc">
	</unit>
	STDOUT

srcml2src --xml --unit "2" - <<< "$nestedfile"

check 3<<< "$sxmlfile2"

srcml2src --xml --unit "2" <<< "$nestedfile"

check 3<<< "$sxmlfile2"
