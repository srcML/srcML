#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# test prefix extraction
define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmnls:lit="http://www.sdml.info/srcML/literal" revision="0.8.0" language="C++" dir="sub" filename="a.cpp">
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$sxmlfile1"

srcml2src sub/a.cpp.xml -p "http://www.sdml.info/srcML/src"
check 3<<< ""

srcml2src sub/a.cpp.xml --prefix "http://www.sdml.info/srcML/src"
check 3<<< ""

srcml2src sub/a.cpp.xml --prefix="http://www.sdml.info/srcML/src"
check 3<<< ""

srcml2src sub/a.cpp.xml -p "http://www.sdml.info/srcML/cpp"
check 3<<< "cpp"

srcml2src sub/a.cpp.xml --prefix "http://www.sdml.info/srcML/cpp"
check 3<<< "cpp"

srcml2src sub/a.cpp.xml --prefix="http://www.sdml.info/srcML/cpp"
check 3<<< "cpp"

srcml2src sub/a.cpp.xml -p "http://www.sdml.info/srcML/literal"
check 3<<< "lit"

srcml2src sub/a.cpp.xml --prefix "http://www.sdml.info/srcML/literal"
check 3<<< "lit"

srcml2src sub/a.cpp.xml --prefix="http://www.sdml.info/srcML/literal"
check 3<<< "lit"

srcml2src sub/a.cpp.xml -p "http://www.cs.uakron.edu/~collard/foo"
check 3<<< ""

srcml2src sub/a.cpp.xml --prefix "http://www.cs.uakron.edu/~collard/foo"
check 3<<< ""

srcml2src sub/a.cpp.xml --prefix="http://www.cs.uakron.edu/~collard/foo"
check 3<<< ""

rmfile sub/a.cpp.xml
