#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	srcmlstart = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	STDOUT
INPUT
cpp = STOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

cppempty= STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

java = STDOUT
<unit language="Java" filename="sub/a.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOU

javaempty = STDOUT
<unit language="Java" filename="sub/a.java"/>
	STDOUT

