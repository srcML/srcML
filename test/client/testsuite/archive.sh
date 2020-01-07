#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test archive
define fileasrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT
xmlcheck "$fileasrcml"

define asrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT
xmlcheck "$asrcml"

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT
xmlcheck "$nestedfile"

createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

# test that two files will output in an archive by default
srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$nestedfile"

srcml sub/a.cpp sub/b.cpp -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$nestedfile"

srcml -o sub/ab.cpp.xml sub/a.cpp sub/b.cpp
check sub/ab.cpp.xml "$nestedfile"

srcml -o sub/ab.cpp.xml sub/a.cpp sub/b.cpp
check sub/ab.cpp.xml "$nestedfile"

# test explicit archive flag
srcml sub/a.cpp --archive
check "$fileasrcml"

srcml sub/a.cpp --archive
check "$fileasrcml"

srcml --archive sub/a.cpp
check "$fileasrcml"

srcml --archive sub/a.cpp
check "$fileasrcml"

srcml sub/a.cpp --archive -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

srcml sub/a.cpp --archive -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

srcml sub/a.cpp -o sub/ab.cpp.xml --archive
check sub/ab.cpp.xml "$fileasrcml"

srcml sub/a.cpp -o sub/ab.cpp.xml --archive
check sub/ab.cpp.xml "$fileasrcml"

srcml --archive sub/a.cpp -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

srcml sub/a.cpp --archive -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

srcml --archive sub/a.cpp -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

srcml --archive sub/a.cpp -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

srcml --archive sub/a.cpp -o sub/ab.cpp.xml
check sub/ab.cpp.xml "$fileasrcml"

srcml --archive -o sub/ab.cpp.xml sub/a.cpp
check sub/ab.cpp.xml "$fileasrcml"

# from standard in
echo -n "a;" | srcml --archive -l C++
check "$asrcml"

echo -n "a;" | srcml --archive -l C++
check "$asrcml"

echo -n "a;" | srcml --archive -o sub/ab.cpp.xml -l C++
check sub/ab.cpp.xml "$asrcml"

echo -n "a;" | srcml --archive -o sub/ab.cpp.xml -l C++
check sub/ab.cpp.xml "$asrcml"

echo -n "a;" | srcml -o sub/ab.cpp.xml --archive -l C++
check sub/ab.cpp.xml "$asrcml"

echo -n "a;" | srcml -o sub/ab.cpp.xml --archive -l C++
check sub/ab.cpp.xml "$asrcml"

echo -n "a;" | srcml -o sub/ab.cpp.xml --archive -l C++
check sub/ab.cpp.xml "$asrcml"

srcml --archive -l C++ < sub/a.cpp
check "$asrcml"

srcml --archive -l C++ < sub/a.cpp
check "$asrcml"

srcml --archive -o sub/ab.cpp.xml -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

srcml --archive -o sub/ab.cpp.xml -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

srcml -o sub/ab.cpp.xml --archive -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

srcml -o sub/ab.cpp.xml --archive -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

srcml -o sub/ab.cpp.xml --archive -l C++ < sub/a.cpp
check sub/ab.cpp.xml "$asrcml"

# test issue #1063: nondeterministic failures when run multiple times
srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"

srcml sub/a.cpp sub/b.cpp
check "$nestedfile"
