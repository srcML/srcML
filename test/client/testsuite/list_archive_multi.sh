#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT
xmlcheck "$srcml"

define output <<- 'STDOUT'
	XML encoding: UTF-8
	    1  C++     1 aa2a72b26cf958d8718a2e9bc6b84679a81d54cb a.cpp
	    2  C++     1 520b48acbdb61e411641fd94359a82686d5591eb b.cpp
	units: 2
	LOC: 2
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# test --list on input archives
srcml --list sub/a.cpp.xml
check "$output"

srcml -L sub/a.cpp.xml
check "$output"

srcml --list < sub/a.cpp.xml
check "$output"

srcml -L < sub/a.cpp.xml
check "$output"
