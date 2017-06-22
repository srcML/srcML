#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# archive of one unit
# test on standard in
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="bar">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

# test on file
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="bar">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$output"
xmlcheck "$fsrcml"
createfile sub/a.cpp "a;"

echo -n "a;" | src2srcml -l C++ --url bar --archive
checkv2 "$output"

echo -n "a;" | src2srcml -l C++ --url=bar --archive
checkv2 "$output"


src2srcml sub/a.cpp --url "bar" --archive
checkv2 "$fsrcml"

src2srcml sub/a.cpp --url="bar" --archive
checkv2 "$fsrcml"

src2srcml --url "bar" sub/a.cpp --archive
checkv2 "$fsrcml"


src2srcml --url="bar" sub/a.cpp --archive
checkv2 "$fsrcml"

src2srcml -l C++ --url 'bar' -o sub/a.cpp.xml sub/a.cpp --archive
checkv2 sub/a.cpp.xml "$fsrcml"

src2srcml --url 'bar' sub/a.cpp -o sub/a.cpp.xml --archive
checkv2 sub/a.cpp.xml "$fsrcml"


# archive of multiple units
# test on file
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="bar">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/b.cpp "b;"

src2srcml sub/a.cpp sub/b.cpp --url "bar" --in-order
checkv2 "$fsrcml"

src2srcml sub/a.cpp sub/b.cpp --url="bar" --in-order
checkv2 "$fsrcml"

src2srcml --url "bar" sub/a.cpp sub/b.cpp --in-order
checkv2 "$fsrcml"

src2srcml --url "bar" sub/a.cpp sub/b.cpp --in-order
checkv2 "$fsrcml"

src2srcml --url="bar" sub/a.cpp sub/b.cpp --in-order
checkv2 "$fsrcml"

src2srcml -l C++ --url 'bar' -o sub/a.cpp.xml sub/a.cpp sub/b.cpp --in-order
checkv2 sub/a.cpp.xml "$fsrcml"

src2srcml --url 'bar' sub/a.cpp sub/b.cpp -o sub/a.cpp.xml --in-order
checkv2 sub/a.cpp.xml "$fsrcml"

