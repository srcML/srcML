#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# neste

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define nestedfilesrc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.srcML.org/srcML/src" revision="REVISION">

	<src:unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt></src:unit>

	<src:unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt></src:unit>

	</src:unit>
	STDOUT

xmlcheck "$output"
xmlcheck "$nestedfile"
xmlcheck "$nestedfilesrc"

createfile sub/a.cpp "
a;"
createfile sub/b.cpp "
b;"

srcml --archive sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

srcml sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$nestedfile"

srcml --xmlns:src=http://www.srcML.org/srcML/src sub/a.cpp sub/b.cpp  -o sub/a.cpp.xml
check sub/a.cpp.xml "$nestedfilesrc"
