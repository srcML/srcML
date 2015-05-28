#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# default
define foosrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<bar:unit xmlns:bar="http://www.srcML.org/srcML/src" xmlns="http://www.foo.com" revision="REVISION">

	<bar:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><bar:expr_stmt><bar:expr><bar:name>a</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	<bar:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><bar:expr_stmt><bar:expr><bar:name>b</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	</bar:unit>
	STDOUT

# this and the other xmlcheck are failing, causing the test to fail.
xmlcheck "$foosrcml"
createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

srcml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp --in-order
check 3<<< "$foosrcml"

srcml sub/a.cpp sub/b.cpp --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" --in-order
check 3<<< "$foosrcml"

srcml sub/a.cpp sub/b.cpp --in-order --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src"
check 3<<< "$foosrcml"

srcml --in-order --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp
check 3<<< "$foosrcml"

srcml --in-order sub/a.cpp sub/b.cpp --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src"
check 3<<< "$foosrcml"

srcml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp --in-order -o sub/a.xml
check sub/a.xml 3<<< "$foosrcml"

srcml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp -o sub/a.xml --in-order
check sub/a.xml 3<<< "$foosrcml"

srcml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" -o sub/a.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.xml 3<<< "$foosrcml"

srcml -o sub/a.xml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp --in-order
check sub/a.xml 3<<< "$foosrcml"

srcml sub/a.cpp sub/b.cpp --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" --in-order -o sub/a.xml
check sub/a.xml 3<<< "$foosrcml"

srcml sub/a.cpp sub/b.cpp --in-order -o sub/a.xml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src"
check sub/a.xml 3<<< "$foosrcml"

srcml sub/a.cpp sub/b.cpp -o sub/a.xml --in-order --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src"
check sub/a.xml 3<<< "$foosrcml"

srcml -o sub/a.xml sub/a.cpp sub/b.cpp --in-order --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src"
check sub/a.xml 3<<< "$foosrcml"

srcml --in-order --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp -o sub/a.xml
check sub/a.xml 3<<< "$foosrcml"

srcml --in-order --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" -o sub/a.xml sub/a.cpp sub/b.cpp
check sub/a.xml 3<<< "$foosrcml"

srcml --in-order -o sub/a.xml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp
check sub/a.xml 3<<< "$foosrcml"

srcml -o sub/a.xml --in-order --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp
check sub/a.xml 3<<< "$foosrcml"

srcml --in-order sub/a.cpp sub/b.cpp --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" -o sub/a.xml
check sub/a.xml 3<<< "$foosrcml"

srcml --in-order sub/a.cpp sub/b.cpp -o sub/a.xml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src"
check sub/a.xml 3<<< "$foosrcml"

srcml --in-order -o sub/a.xml sub/a.cpp sub/b.cpp --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src"
check sub/a.xml 3<<< "$foosrcml"

srcml -o sub/a.xml --in-order sub/a.cpp sub/b.cpp --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src"
check sub/a.xml 3<<< "$foosrcml"

# with prefix
define fooprefixsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="http://www.foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$fooprefixsrcml"

srcml --xmlns:foo="http://www.foo.com" sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fooprefixsrcml"

srcml sub/a.cpp sub/b.cpp --xmlns:foo="http://www.foo.com" --in-order
check 3<<< "$fooprefixsrcml"

srcml sub/a.cpp sub/b.cpp --in-order --xmlns:foo="http://www.foo.com"
check 3<<< "$fooprefixsrcml"

srcml --in-order --xmlns:foo="http://www.foo.com" sub/a.cpp sub/b.cpp
check 3<<< "$fooprefixsrcml"

srcml --in-order sub/a.cpp sub/b.cpp --xmlns:foo="http://www.foo.com"
check 3<<< "$fooprefixsrcml"

srcml --xmlns:foo="http://www.foo.com" sub/a.cpp sub/b.cpp --in-order -o sub/a.xml
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --xmlns:foo="http://www.foo.com" sub/a.cpp sub/b.cpp -o sub/a.xml --in-order
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --xmlns:foo="http://www.foo.com" -o sub/a.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml -o sub/a.xml --xmlns:foo="http://www.foo.com" sub/a.cpp sub/b.cpp --in-order
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml sub/a.cpp sub/b.cpp --xmlns:foo="http://www.foo.com" --in-order -o sub/a.xml
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml sub/a.cpp sub/b.cpp --in-order -o sub/a.xml --xmlns:foo="http://www.foo.com"
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml sub/a.cpp sub/b.cpp -o sub/a.xml --in-order --xmlns:foo="http://www.foo.com"
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml -o sub/a.xml sub/a.cpp sub/b.cpp --in-order --xmlns:foo="http://www.foo.com"
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --in-order --xmlns:foo="http://www.foo.com" sub/a.cpp sub/b.cpp -o sub/a.xml
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --in-order --xmlns:foo="http://www.foo.com" -o sub/a.xml sub/a.cpp sub/b.cpp
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --in-order -o sub/a.xml --xmlns:foo="http://www.foo.com" sub/a.cpp sub/b.cpp
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml -o sub/a.xml --in-order --xmlns:foo="http://www.foo.com" sub/a.cpp sub/b.cpp
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --in-order sub/a.cpp sub/b.cpp --xmlns:foo="http://www.foo.com" -o sub/a.xml
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --in-order sub/a.cpp sub/b.cpp -o sub/a.xml --xmlns:foo="http://www.foo.com"
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml --in-order -o sub/a.xml sub/a.cpp sub/b.cpp --xmlns:foo="http://www.foo.com"
check sub/a.xml 3<<< "$fooprefixsrcml"

srcml -o sub/a.xml --in-order sub/a.cpp sub/b.cpp --xmlns:foo="http://www.foo.com"
check sub/a.xml 3<<< "$fooprefixsrcml"

