#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srcmla <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

define srcmlb <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$srcmla"
xmlcheck "$srcmlb"

# test --no-archive
createfile a.cpp "a;"
createfile b.cpp "b;"

srcml --to-dir . a.cpp b.cpp
check a.cpp.xml "$srcmla"
check b.cpp.xml "$srcmlb"

srcml --to-dir . a.cpp b.cpp
check a.cpp.xml "$srcmla"

srcml a.cpp b.cpp --to-dir .
check a.cpp.xml "$srcmla"

srcml a.cpp b.cpp --to-dir .
check a.cpp.xml "$srcmla"

srcml a.cpp b.cpp --to-dir .
check a.cpp.xml "$srcmla"
