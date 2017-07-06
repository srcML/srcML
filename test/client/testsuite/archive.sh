#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test archive
define fileasrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">
	
	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT
xmlcheck "$fileasrcml"

define asrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">
	
	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT
xmlcheck "$asrcml"

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT
xmlcheck "$nestedfile"

mkdir sub

createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

# test that two files will output in an archive by default
src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml --in-order sub/a.cpp sub/b.cpp
check "$nestedfile"

src2srcml sub/a.cpp --in-order sub/b.cpp
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp -o sub/ab.cpp.xml --in-order
check sub/ab.cpp.xml "$nestedfile"

src2srcml -o sub/ab.cpp.xml --in-order sub/a.cpp sub/b.cpp
check sub/ab.cpp.xml "$nestedfile"

src2srcml --in-order -o sub/ab.cpp.xml sub/a.cpp sub/b.cpp
check sub/ab.cpp.xml "$nestedfile"

# test explicit archive flag
src2srcml sub/a.cpp --archive --in-order
check "$fileasrcml"

src2srcml sub/a.cpp --in-order --archive
check "$fileasrcml"

src2srcml --archive --in-order sub/a.cpp
check "$fileasrcml"

src2srcml --in-order --archive sub/a.cpp
check "$fileasrcml"

src2srcml sub/a.cpp --archive --in-order -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

src2srcml sub/a.cpp --in-order --archive -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

src2srcml sub/a.cpp --in-order -o sub/ab.cpp.xml --archive
check sub/ab.cpp.xml "$fileasrcml"

src2srcml --in-order sub/a.cpp -o sub/ab.cpp.xml --archive
check sub/ab.cpp.xml "$fileasrcml"

src2srcml --in-order --archive sub/a.cpp -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

src2srcml --in-order sub/a.cpp --archive -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

src2srcml --archive --in-order sub/a.cpp -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

src2srcml --archive sub/a.cpp --in-order -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

src2srcml --archive sub/a.cpp -o sub/ab.cpp.xml --in-order
check sub/ab.cpp.xml "$fileasrcml"

src2srcml --archive -o sub/ab.cpp.xml --in-order sub/a.cpp
check sub/ab.cpp.xml "$fileasrcml"

# from standard in
echo -n "a;" | srcml2src --archive -l C++ --in-order
check "$asrcml"

echo -n "a;" | srcml2src --in-order --archive -l C++
check "$asrcml"

echo -n "a;" | srcml2src --archive --in-order -o sub/ab.cpp.xml -l C++
check sub/ab.cpp.xml "$asrcml"

echo -n "a;" | srcml2src --in-order --archive -o sub/ab.cpp.xml -l C++
check sub/ab.cpp.xml "$asrcml"

echo -n "a;" | srcml2src --in-order -o sub/ab.cpp.xml --archive -l C++
check sub/ab.cpp.xml "$asrcml"

echo -n "a;" | srcml2src -o sub/ab.cpp.xml --archive --in-order -l C++
check sub/ab.cpp.xml "$asrcml"

echo -n "a;" | srcml2src -o sub/ab.cpp.xml --in-order --archive -l C++
check sub/ab.cpp.xml "$asrcml"

srcml2src --archive -l C++ --in-order < sub/a.cpp
check "$asrcml"

srcml2src --in-order --archive -l C++ < sub/a.cpp
check "$asrcml"

srcml2src --archive --in-order -o sub/ab.cpp.xml -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

srcml2src --in-order --archive -o sub/ab.cpp.xml -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

srcml2src --in-order -o sub/ab.cpp.xml --archive -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

srcml2src -o sub/ab.cpp.xml --archive --in-order -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

srcml2src -o sub/ab.cpp.xml --in-order --archive -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"


# test issue #1063: nondeterministic failures when run multiple times
src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"

src2srcml sub/a.cpp sub/b.cpp --in-order
check "$nestedfile"
