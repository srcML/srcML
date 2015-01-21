#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	2
	STDOUT

# test --count on srcml files
createfile sub/a.cpp.xml "$srcml"

srcml2src --count sub/a.cpp.xml
check 3<<< "$output"

srcml2src --count < sub/a.cpp.xml
check 3<<< "$output"

# test --count on src files
createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

src2srcml --count sub/a.cpp sub/b.cpp
check 3<<< "$output"

src2srcml sub/a.cpp sub/b.cpp --count
check 3<<< "$output"
