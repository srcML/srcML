#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test directory input
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="AspectJ" filename="dir/file.aj" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="dir/file.c" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="dir/file.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="Java" filename="dir/file.java" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile dir/file.aj "\na;"
createfile dir/file.c  "\na;"
createfile dir/file.cpp "\na;"
createfile dir/file.java "\na;"

src2srcml dir --quiet -o dir/dir.xml --in-order
check dir/dir.xml 3<<< "$output"

src2srcml dir --quiet --in-order -o dir/dir.xml
check dir/dir.xml 3<<< "$output"

src2srcml --in-order dir --quiet -o dir/dir.xml
check dir/dir.xml 3<<< "$output"

src2srcml dir --quiet --in-order
check 3<<< "$output"
