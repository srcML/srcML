#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# neste 

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define nestedfilesrc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.sdml.info/srcML/src" revision="0.8.0">

	<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt></src:unit>

	<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt></src:unit>

	</src:unit>
	STDOUT

createfile sub/a.cpp "
a;"
createfile sub/b.cpp "
b;"

rmfile sub/a.cpp.xml
src2srcml --archive sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output"

rmfile sub/a.cpp.xml
src2srcml sub/a.cpp sub/b.cpp --ordered -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$nestedfile"

rmfile sub/a.cpp.xml
src2srcml --xmlns:src=http://www.sdml.info/srcML/src sub/a.cpp sub/b.cpp --ordered  -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$nestedfilesrc"
