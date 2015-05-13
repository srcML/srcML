#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test src-version attribute on archive of one
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" version="1.0">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" version="1.0" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" version="1.0">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" version="1.0" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/a.cpp "a;"

# standard input
src2srcml -l C++ -s '1.0' -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml -l C++ -s '1.0' --archive -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml -l C++ --archive -s '1.0' -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --archive -l C++ -s '1.0' -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml -l C++ -s '1.0' --archive < sub/a.cpp
check 3<<< "$srcml"

src2srcml -l C++ --archive -s '1.0' < sub/a.cpp
check 3<<< "$srcml"

src2srcml --archive -l C++ -s '1.0' < sub/a.cpp
check 3<<< "$srcml"

# from a file
src2srcml sub/a.cpp -s 1.0 --archive
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --archive -s 1.0
check 3<<< "$fsrcml"

src2srcml --archive sub/a.cpp -s 1.0
check 3<<< "$fsrcml"

src2srcml -s 1.0 sub/a.cpp --archive
check 3<<< "$fsrcml"

src2srcml -s 1.0 --archive sub/a.cpp
check 3<<< "$fsrcml"

src2srcml --archive -s 1.0 sub/a.cpp
check 3<<< "$fsrcml"

src2srcml sub/a.cpp -s 1.0 --archive
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --archive -s 1.0
check 3<<< "$fsrcml"

src2srcml --archive sub/a.cpp -s 1.0
check 3<<< "$fsrcml"

src2srcml -s 1.0 sub/a.cpp --archive
check 3<<< "$fsrcml"

src2srcml -s 1.0 --archive sub/a.cpp
check 3<<< "$fsrcml"

src2srcml --archive -s 1.0 sub/a.cpp
check 3<<< "$fsrcml"


# test src-version attribute an archive of more than one
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" version="1.0">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" version="1.0" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" version="1.0" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/b.cpp "b;"

src2srcml sub/a.cpp sub/b.cpp -s "1.0" --in-order
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --in-order -s "1.0"
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --in-order sub/b.cpp -s "1.0"
check 3<<< "$fsrcml"

src2srcml --in-order sub/a.cpp sub/b.cpp -s "1.0"
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --src-version "1.0" --in-order
check 3<<< "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --in-order --src-version="1.0"
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --in-order sub/b.cpp --src-version="1.0"
check 3<<< "$fsrcml"

src2srcml --in-order sub/a.cpp sub/b.cpp --src-version="1.0"
check 3<<< "$fsrcml"

src2srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp --in-order sub/b.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -l C++ -s '1.0' -o sub/a.cpp.xml --in-order sub/a.cpp sub/b.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -l C++ -s '1.0' --in-order -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -l C++ --in-order -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml --in-order -l C++ -s '1.0' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -s '1.0' sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --in-order
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -s '1.0' sub/a.cpp sub/b.cpp --in-order -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -s '1.0' sub/a.cpp --in-order sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -s '1.0' --in-order sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml --in-order -s '1.0' sub/a.cpp sub/b.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

