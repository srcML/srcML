#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test enabling hash
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$fsrcml"
createfile sub/a.cpp "a;"

# from a file
srcml sub/a.cpp --archive
check "$fsrcml"

srcml sub/a.cpp --hash --archive
check "$fsrcml"

srcml --hash sub/a.cpp --archive
check "$fsrcml"

srcml sub/a.cpp --hash -o sub/a.xml --archive
check sub/a.xml "$fsrcml"

srcml sub/a.cpp -o sub/a.xml --hash --archive
check sub/a.xml "$fsrcml"

srcml -o sub/a.xml sub/a.cpp --hash --archive
check sub/a.xml "$fsrcml"

srcml --hash sub/a.cpp -o sub/a.xml --archive
check sub/a.xml "$fsrcml"

srcml --hash -o sub/a.xml sub/a.cpp --archive
check sub/a.xml "$fsrcml"

srcml -o sub/a.xml --hash sub/a.cpp --archive
check sub/a.xml "$fsrcml"

# standard input
srcml -l C++ --hash --archive < sub/a.cpp
check "$srcml"

srcml --hash -l C++ --archive < sub/a.cpp
check "$srcml"

srcml -l C++ --hash -o sub/a.xml --archive < sub/a.cpp
check sub/a.xml "$srcml"

srcml -l C++ -o sub/a.xml --hash --archive < sub/a.cpp
check sub/a.xml "$srcml"

srcml -o sub/a.xml -l C++ --hash --archive < sub/a.cpp
check sub/a.xml "$srcml"

srcml --hash -l C++ -o sub/a.xml --archive < sub/a.cpp
check sub/a.xml "$srcml"

srcml --hash -o sub/a.xml -l C++ --archive < sub/a.cpp
check sub/a.xml "$srcml"

srcml -o sub/a.xml --hash -l C++ --archive < sub/a.cpp
check sub/a.xml "$srcml"

# with more than one unit in the archive
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$fsrcml"
createfile sub/b.cpp "b;"

# from a file
srcml sub/a.cpp sub/b.cpp
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --hash
check "$fsrcml"

srcml --hash sub/a.cpp sub/b.cpp
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --hash -o sub/a.xml
check sub/a.xml "$fsrcml"

srcml sub/a.cpp sub/b.cpp -o sub/a.xml --hash
check sub/a.xml "$fsrcml"

srcml -o sub/a.xml sub/a.cpp sub/b.cpp --hash
check sub/a.xml "$fsrcml"

srcml --hash sub/a.cpp sub/b.cpp -o sub/a.xml
check sub/a.xml "$fsrcml"

srcml --hash -o sub/a.xml sub/a.cpp sub/b.cpp
check sub/a.xml "$fsrcml"

srcml -o sub/a.xml --hash sub/a.cpp sub/b.cpp
check sub/a.xml "$fsrcml"
