#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	srcmlstart = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	STDOUT
INPUT
aj = STDOUT
<unit language="AspectJ" filename="dir/file.aj">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

c = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="dir/file.c">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

cpp = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="dir/file.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

java = STDOUT
<unit language="Java" filename="dir/file.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

