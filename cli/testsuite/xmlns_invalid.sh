#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh
set e+

# can't overwrite default namespace without redefining src
define foosrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

srcml --xmlns=foo.com sub/a.cpp sub/b.cpp --in-order
check_exit 3

srcml sub/a.cpp sub/b.cpp --xmlns=foo.com --in-order
check_exit 3

srcml sub/a.cpp sub/b.cpp --in-order --xmlns=foo.com
check_exit 3

srcml --in-order --xmlns=foo.com sub/a.cpp sub/b.cpp
check_exit 3

srcml --in-order sub/a.cpp sub/b.cpp --xmlns=foo.com
check_exit 3

srcml --xmlns=foo.com sub/a.cpp sub/b.cpp --in-order -o sub/a.xml
check_exit 3

srcml --xmlns=foo.com sub/a.cpp sub/b.cpp -o sub/a.xml --in-order
check_exit 3

srcml --xmlns=foo.com -o sub/a.xml sub/a.cpp sub/b.cpp --in-order
check_exit 3

srcml -o sub/a.xml --xmlns=foo.com sub/a.cpp sub/b.cpp --in-order
check_exit 3

srcml sub/a.cpp sub/b.cpp --xmlns=foo.com --in-order -o sub/a.xml
check_exit 3

srcml sub/a.cpp sub/b.cpp --in-order -o sub/a.xml --xmlns=foo.com
check_exit 3

srcml sub/a.cpp sub/b.cpp -o sub/a.xml --in-order --xmlns=foo.com
check_exit 3

srcml -o sub/a.xml sub/a.cpp sub/b.cpp --in-order --xmlns=foo.com
check_exit 3

srcml --in-order --xmlns=foo.com sub/a.cpp sub/b.cpp -o sub/a.xml
check_exit 3

srcml --in-order --xmlns=foo.com -o sub/a.xml sub/a.cpp sub/b.cpp
check_exit 3

srcml --in-order -o sub/a.xml --xmlns=foo.com sub/a.cpp sub/b.cpp
check_exit 3

srcml -o sub/a.xml --in-order --xmlns=foo.com sub/a.cpp sub/b.cpp
check_exit 3

srcml --in-order sub/a.cpp sub/b.cpp --xmlns=foo.com -o sub/a.xml
check_exit 3

srcml --in-order sub/a.cpp sub/b.cpp -o sub/a.xml --xmlns=foo.com
check_exit 3

srcml --in-order -o sub/a.xml sub/a.cpp sub/b.cpp --xmlns=foo.com
check_exit 3

srcml -o sub/a.xml --in-order sub/a.cpp sub/b.cpp --xmlns=foo.com
check_exit 3

# with prefix
define fooprefixsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

srcml --xmlns:foo=foo.com sub/a.cpp sub/b.cpp --in-order
check_exit 3

srcml sub/a.cpp sub/b.cpp --xmlns:foo=foo.com --in-order
check_exit 3

srcml sub/a.cpp sub/b.cpp --in-order --xmlns:foo=foo.com
check_exit 3

srcml --in-order --xmlns:foo=foo.com sub/a.cpp sub/b.cpp
check_exit 3

srcml --in-order sub/a.cpp sub/b.cpp --xmlns:foo=foo.com
check_exit 3

srcml --xmlns:foo=foo.com sub/a.cpp sub/b.cpp --in-order -o sub/a.xml
check_exit 3

srcml --xmlns:foo=foo.com sub/a.cpp sub/b.cpp -o sub/a.xml --in-order
check_exit 3

srcml --xmlns:foo=foo.com -o sub/a.xml sub/a.cpp sub/b.cpp --in-order
check_exit 3

srcml -o sub/a.xml --xmlns:foo=foo.com sub/a.cpp sub/b.cpp --in-order
check_exit 3

srcml sub/a.cpp sub/b.cpp --xmlns:foo=foo.com --in-order -o sub/a.xml
check_exit 3

srcml sub/a.cpp sub/b.cpp --in-order -o sub/a.xml --xmlns:foo=foo.com
check_exit 3

srcml sub/a.cpp sub/b.cpp -o sub/a.xml --in-order --xmlns:foo=foo.com
check_exit 3

srcml -o sub/a.xml sub/a.cpp sub/b.cpp --in-order --xmlns:foo=foo.com
check_exit 3

srcml --in-order --xmlns:foo=foo.com sub/a.cpp sub/b.cpp -o sub/a.xml
check_exit 3

srcml --in-order --xmlns:foo=foo.com -o sub/a.xml sub/a.cpp sub/b.cpp
check_exit 3

srcml --in-order -o sub/a.xml --xmlns:foo=foo.com sub/a.cpp sub/b.cpp
check_exit 3

srcml -o sub/a.xml --in-order --xmlns:foo=foo.com sub/a.cpp sub/b.cpp
check_exit 3

srcml --in-order sub/a.cpp sub/b.cpp --xmlns:foo=foo.com -o sub/a.xml
check_exit 3

srcml --in-order sub/a.cpp sub/b.cpp -o sub/a.xml --xmlns:foo=foo.com
check_exit 3

srcml --in-order -o sub/a.xml sub/a.cpp sub/b.cpp --xmlns:foo=foo.com
check_exit 3

srcml -o sub/a.xml --in-order sub/a.cpp sub/b.cpp --xmlns:foo=foo.com
check_exit 3

