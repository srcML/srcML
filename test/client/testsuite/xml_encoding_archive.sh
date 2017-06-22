#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test encoding flag on an archive of one
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$fsrcml"
createfile sub/a.cpp "a;"

src2srcml sub/a.cpp -x "ISO-8859-1" --archive 
check "$fsrcml"

src2srcml sub/a.cpp --xml-encoding="ISO-8859-1" --archive 
check "$fsrcml"

src2srcml --xml-encoding "ISO-8859-1" sub/a.cpp --archive 
check "$fsrcml"

src2srcml -l C++ -x "ISO-8859-1" -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

src2srcml -l C++ -o sub/a.cpp.xml -x "ISO-8859-1" --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

src2srcml -o sub/a.cpp.xml -l C++ -x "ISO-8859-1" --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

src2srcml -x "ISO-8859-1" -l C++ -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

src2srcml -x "ISO-8859-1" sub/a.cpp -o sub/a.cpp.xml --archive 
check sub/a.cpp.xml "$fsrcml"

# test encoding flag on an archive of more than one
define srcmlb <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define fsrcmlb <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$srcmlb"
xmlcheck "$fsrcmlb"
createfile sub/b.cpp "b;"

src2srcml sub/a.cpp sub/b.cpp --in-order -x "ISO-8859-1"
check "$fsrcmlb"

src2srcml sub/a.cpp sub/b.cpp --in-order --xml-encoding="ISO-8859-1"
check "$fsrcmlb"

src2srcml --xml-encoding "ISO-8859-1" sub/a.cpp sub/b.cpp --in-order
check "$fsrcmlb"

src2srcml --xml-encoding "ISO-8859-1" sub/a.cpp --in-order sub/b.cpp
check "$fsrcmlb"

src2srcml --xml-encoding "ISO-8859-1" --in-order sub/a.cpp sub/b.cpp
check "$fsrcmlb"

src2srcml --in-order --xml-encoding "ISO-8859-1" sub/a.cpp sub/b.cpp
check "$fsrcmlb"

src2srcml --in-order sub/a.cpp --xml-encoding "ISO-8859-1" sub/b.cpp
check "$fsrcmlb"

src2srcml -x "ISO-8859-1" sub/a.cpp sub/b.cpp --in-order -o sub/a.cpp.xml
check sub/a.cpp.xml "$fsrcmlb"

src2srcml -o sub/a.cpp.xml -x "ISO-8859-1" sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml "$fsrcmlb"

src2srcml -o sub/a.cpp.xml sub/a.cpp -x "ISO-8859-1" sub/b.cpp --in-order
check sub/a.cpp.xml "$fsrcmlb"

src2srcml -o sub/a.cpp.xml sub/a.cpp sub/b.cpp -x "ISO-8859-1" --in-order
check sub/a.cpp.xml "$fsrcmlb"

src2srcml -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order -x "ISO-8859-1"
check sub/a.cpp.xml "$fsrcmlb"

