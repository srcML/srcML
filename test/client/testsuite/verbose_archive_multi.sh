#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test verbose on an archive
define sfile <<< "a;"

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define foutput <<- 'STDERR'
	XML encoding:  UTF-8
	    1 C++      1 a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8 sub/a.cpp
	    2 C++      1 9a1e1d3d0e27715d29bcfbf72b891b3ece985b36 sub/b.cpp

	Source Files: 2	Other Files: 0	Errors: 0	Total Files: 2
	STDERR

xmlcheck "$fsrcml"

createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"
createfile sub/a.cpp.xml "$fsrcml"

# from a file
srcml sub/a.cpp sub/b.cpp --archive --verbose
check "$fsrcml" "$foutput"

srcml sub/a.cpp sub/b.cpp --verbose --archive
check "$fsrcml" "$foutput"

srcml sub/a.cpp sub/b.cpp --verbose --archive
check "$fsrcml" "$foutput"

srcml sub/a.cpp --verbose sub/b.cpp --archive
check "$fsrcml" "$foutput"

srcml --verbose sub/a.cpp sub/b.cpp --archive
check "$fsrcml" "$foutput"

srcml sub/a.cpp sub/b.cpp --archive --verbose -o sub/c.cpp.xml
check sub/c.cpp.xml "$fsrcml" "$foutput"

srcml sub/a.cpp sub/b.cpp --archive -o sub/c.cpp.xml --verbose
check sub/c.cpp.xml "$fsrcml" "$foutput"

srcml sub/a.cpp sub/b.cpp -o sub/c.cpp.xml --archive --verbose
check sub/c.cpp.xml "$fsrcml" "$foutput"

srcml sub/a.cpp sub/b.cpp -o sub/c.cpp.xml --archive --verbose
check sub/c.cpp.xml "$fsrcml" "$foutput"

srcml sub/a.cpp -o sub/c.cpp.xml sub/b.cpp --archive --verbose
check sub/c.cpp.xml "$fsrcml" "$foutput"

srcml -o sub/c.cpp.xml sub/a.cpp sub/b.cpp --archive --verbose
check sub/c.cpp.xml "$fsrcml" "$foutput"

