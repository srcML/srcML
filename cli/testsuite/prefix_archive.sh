#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# archive of one unit
define input <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	INPUT

createfile sub/a.xml "$input"

# from a file
srcml -U 1 -X -p "http://www.srcML.org/srcML/cpp" sub/a.xml
check 3<<< "cpp"

srcml sub/a.xml -U 1 -X -p "http://www.srcML.org/srcML/cpp"
check 3<<< "cpp"

srcml -p "http://www.srcML.org/srcML/src" sub/a.xml
check 3<<< ""

srcml sub/a.xml -p "http://www.srcML.org/srcML/src"
check 3<<< ""

srcml -p "foo.com" sub/a.xml
check 3<<< "foo"

srcml sub/a.xml -p "foo.com"
check 3<<< "foo"

srcml -p "nonexistent.com" sub/a.xml
check_null

srcml sub/a.xml -p "nonexistent.com"
check_null

# standard in
srcml2src -U 1 -X -p "http://www.srcML.org/srcML/cpp" <<< "$input"
check 3<<< "cpp"

srcml2src -U 1 -X --prefix "http://www.srcML.org/srcML/cpp" <<< "$input"
check 3<<< "cpp"

srcml2src -U 1 -X --prefix="http://www.srcML.org/srcML/cpp" <<< "$input"
check 3<<< "cpp"

srcml2src -p "http://www.srcML.org/srcML/src" <<< "$input"
check 3<<< ""

srcml2src --prefix "http://www.srcML.org/srcML/src" <<< "$input"
check 3<<< ""

srcml2src --prefix="http://www.srcML.org/srcML/src" <<< "$input"
check 3<<< ""


# archive of multiple units
define archive_multi <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	INPUT

createfile sub/archive_multi.xml "$archive_multi"

srcml -U 1 -X -p "http://www.srcML.org/srcML/cpp" sub/archive_multi.xml
check 3<<< "cpp"

srcml sub/archive_multi.xml -U 1 -X -p "http://www.srcML.org/srcML/cpp"
check 3<<< "cpp"

srcml -p "http://www.srcML.org/srcML/src" sub/archive_multi.xml
check 3<<< ""

srcml sub/archive_multi.xml -p "http://www.srcML.org/srcML/src"
check 3<<< ""

srcml -p "foo.com" sub/archive_multi.xml
check 3<<< "foo"

srcml sub/archive_multi.xml -p "foo.com"
check 3<<< "foo"

srcml -p "nonexistent.com" sub/archive_multi.xml
check_null

srcml sub/archive_multi.xml -p "nonexistent.com"
check_null

# standard in
srcml2src -U 1 -X  -p "http://www.srcML.org/srcML/cpp" <<< "$archive_multi"
check 3<<< "cpp"

srcml2src -U 1 -X --prefix "http://www.srcML.org/srcML/cpp" <<< "$archive_multi"
check 3<<< "cpp"

srcml2src -U 1 -X --prefix="http://www.srcML.org/srcML/cpp" <<< "$archive_multi"
check 3<<< "cpp"

srcml2src -p "http://www.srcML.org/srcML/src" <<< "$archive_multi"
check 3<<< ""

srcml2src --prefix "http://www.srcML.org/srcML/src" <<< "$archive_multi"
check 3<<< ""

srcml2src --prefix="http://www.srcML.org/srcML/src" <<< "$archive_multi"
check 3<<< ""

