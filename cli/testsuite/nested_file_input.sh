#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# neste 

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define nestedfilesrc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.srcML.org/srcML/src" revision="REVISION">

	<src:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt></src:unit>

	<src:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt></src:unit>

	</src:unit>
	STDOUT

createfile sub/a.cpp "
a;"
createfile sub/b.cpp "
b;"

src2srcml --archive sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output"

src2srcml sub/a.cpp sub/b.cpp --in-order -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$nestedfile"

src2srcml --xmlns:src=http://www.srcML.org/srcML/src sub/a.cpp sub/b.cpp --in-order  -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$nestedfilesrc"
