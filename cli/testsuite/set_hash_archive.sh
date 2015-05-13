#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test enabling hash
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/a.cpp "a;"

# from a file
src2srcml sub/a.cpp --archive
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --hash --archive
check 3<<< "$fsrcml"

src2srcml --hash sub/a.cpp --archive
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --hash -o sub/a.xml --archive
check sub/a.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp -o sub/a.xml --hash --archive
check sub/a.xml 3<<< "$fsrcml"

src2srcml -o sub/a.xml sub/a.cpp --hash --archive
check sub/a.xml 3<<< "$fsrcml"

src2srcml --hash sub/a.cpp -o sub/a.xml --archive
check sub/a.xml 3<<< "$fsrcml"

src2srcml --hash -o sub/a.xml sub/a.cpp --archive
check sub/a.xml 3<<< "$fsrcml"

src2srcml -o sub/a.xml --hash sub/a.cpp --archive
check sub/a.xml 3<<< "$fsrcml"

# standard input
src2srcml -l C++ --hash --archive < sub/a.cpp
check 3<<< "$srcml"

src2srcml --hash -l C++ --archive < sub/a.cpp
check 3<<< "$srcml"

src2srcml -l C++ --hash -o sub/a.xml --archive < sub/a.cpp
check sub/a.xml 3<<< "$srcml"

src2srcml -l C++ -o sub/a.xml --hash --archive < sub/a.cpp
check sub/a.xml 3<<< "$srcml"

src2srcml -o sub/a.xml -l C++ --hash --archive < sub/a.cpp
check sub/a.xml 3<<< "$srcml"

src2srcml --hash -l C++ -o sub/a.xml --archive < sub/a.cpp
check sub/a.xml 3<<< "$srcml"

src2srcml --hash -o sub/a.xml -l C++ --archive < sub/a.cpp
check sub/a.xml 3<<< "$srcml"

src2srcml -o sub/a.xml --hash -l C++ --archive < sub/a.cpp
check sub/a.xml 3<<< "$srcml"

# with more than one unit in the archive
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/b.cpp "b;"

# from a file
src2srcml sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --hash --in-order
check 3<<< "$fsrcml"

src2srcml --hash sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --hash -o sub/a.xml --in-order
check sub/a.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp -o sub/a.xml --hash --in-order
check sub/a.xml 3<<< "$fsrcml"

src2srcml -o sub/a.xml sub/a.cpp sub/b.cpp --hash --in-order
check sub/a.xml 3<<< "$fsrcml"

src2srcml --hash sub/a.cpp sub/b.cpp -o sub/a.xml --in-order
check sub/a.xml 3<<< "$fsrcml"

src2srcml --hash -o sub/a.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.xml 3<<< "$fsrcml"

src2srcml -o sub/a.xml --hash sub/a.cpp sub/b.cpp --in-order
check sub/a.xml 3<<< "$fsrcml"
