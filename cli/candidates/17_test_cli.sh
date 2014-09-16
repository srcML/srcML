#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define sxmlfile1 <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
	</unit>
	STDIN

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

srcml2src --xml --unit "1" - <<< "$nestedfile"

check 3<<< "$sxmlfile1"

srcml2src --xml --unit "1" <<< "$nestedfile"

check 3<<< "$sxmlfile1"

define sxmlfile2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	</unit>
	STDOUT

srcml2src --xml --unit "2" - <<< "$nestedfile"

check 3<<< "$sxmlfile2"

srcml2src --xml --unit "2" <<< "$nestedfile"

check 3<<< "$sxmlfile2"


