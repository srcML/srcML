#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srca <<- 'STDOUT'
	a;
	STDOUT

define srcb <<- 'STDOUT'
	b;
	STDOUT

define srcab <<- 'STDOUT'
	a;
	b;
	STDOUT

# multi archive
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

# src --> srcml : input srcml single file
xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

echo "a;" > expected_output
echo -en '\0' >> expected_output
echo "b;" >> expected_output

srcml --output-src sub/a.cpp.xml > ab
cmp --verbose ab expected_output
rm ab

srcml -S sub/a.cpp.xml > ab
cmp --verbose ab expected_output
rm ab

srcml -U 1 --output-src sub/a.cpp.xml
check "$srca"

srcml -U 1 -S sub/a.cpp.xml
check "$srca"

srcml -U 2 --output-src sub/a.cpp.xml
check "$srcb"

srcml -U 2 -S sub/a.cpp.xml
check "$srcb"

cat sub/a.cpp.xml | srcml -S > ab
cmp --verbose ab expected_output
rm ab

srcml -U 1 -S < sub/a.cpp.xml
check "$srca"

srcml -U 2 -S < sub/a.cpp.xml
check "$srcb"

srcml -S sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srca"

srcml -S -U 1 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srca"

srcml -S -U 2 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srcb"

srcml -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -S -U 1 -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 1 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 2 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srcb"

srcml -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 1 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 2 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srcb"

# single archive
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

# srcml --> src : input srcml single file
xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

srcml --output-src sub/a.cpp.xml
check "$srca"

srcml -S sub/a.cpp.xml
check "$srca"

srcml -U 1 --output-src sub/a.cpp.xml
check "$srca"

srcml -U 1 -S sub/a.cpp.xml
check "$srca"

srcml -S < sub/a.cpp.xml
check "$srca"

srcml -U 1 -S < sub/a.cpp.xml
check "$srca"

srcml -S sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srca"

srcml -S -U 1 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$srca"

srcml -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -S -U 1 -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 1 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srca"

srcml -U 1 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$srca"
