#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test src-version attribute on archive of one
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" version="1.0">

	<unit revision="REVISION" language="C++" version="1.0" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" version="1.0">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" version="1.0" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$fsrcml"
createfile sub/a.cpp "a;"

# standard input
srcml -l C++ -s '1.0' -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml -l C++ -s '1.0' --archive -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml -l C++ --archive -s '1.0' -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml --archive -l C++ -s '1.0' -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml -l C++ -s '1.0' --archive < sub/a.cpp
check "$srcml"

srcml -l C++ --archive -s '1.0' < sub/a.cpp
check "$srcml"

srcml --archive -l C++ -s '1.0' < sub/a.cpp
check "$srcml"

# from a file
srcml sub/a.cpp -s 1.0 --archive
check "$fsrcml"

srcml sub/a.cpp --archive -s 1.0
check "$fsrcml"

srcml --archive sub/a.cpp -s 1.0
check "$fsrcml"

srcml -s 1.0 sub/a.cpp --archive
check "$fsrcml"

srcml -s 1.0 --archive sub/a.cpp
check "$fsrcml"

srcml --archive -s 1.0 sub/a.cpp
check "$fsrcml"

srcml sub/a.cpp -s 1.0 --archive
check "$fsrcml"

srcml sub/a.cpp --archive -s 1.0
check "$fsrcml"

srcml --archive sub/a.cpp -s 1.0
check "$fsrcml"

srcml -s 1.0 sub/a.cpp --archive
check "$fsrcml"

srcml -s 1.0 --archive sub/a.cpp
check "$fsrcml"

srcml --archive -s 1.0 sub/a.cpp
check "$fsrcml"

# test src-version attribute an archive of more than one
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" version="1.0">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" version="1.0" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" version="1.0" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/b.cpp "b;"

srcml sub/a.cpp sub/b.cpp -s "1.0"
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp -s "1.0"
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp -s "1.0"
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp -s "1.0"
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --src-version "1.0"
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --src-version="1.0"
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --src-version="1.0"
check "$fsrcml"

srcml sub/a.cpp sub/b.cpp --src-version="1.0"
check "$fsrcml"

srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml "$fsrcml"

srcml -s '1.0' sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml -s '1.0' sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml -s '1.0' sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml -s '1.0' sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

srcml -s '1.0' sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcml"

