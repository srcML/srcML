#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# files from
define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">
	
	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/a.cpp "
a;"

createfile sub/b.cpp "
b;"

createfile sub/filelistab "sub/a.cpp
sub/b.cpp
"

src2srcml --files-from "sub/filelistab" --in-order -o sub/both.xml
check sub/both.xml 3<<< "$nestedfile"

src2srcml --files-from "sub/filelistab" --in-order
check 3<<< "$nestedfile"
