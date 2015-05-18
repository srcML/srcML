#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# text flag with empty input
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

src2srcml -t "" -l "C++"
check 3<<< "$srcml"

echo -n "" | src2srcml -l "C++"
check 3<<< "$srcml"

src2srcml -t "" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml -l C++ -t "" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

# simple input
define asrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

src2srcml -t "a;" -l "C++"
check 3<<< "$asrcml"

echo -n "a;" | src2srcml -l "C++"
check 3<<< "$asrcml"

src2srcml -t "a;" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$asrcml"

src2srcml -l C++ -t "a;" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$asrcml"

# embedded newline
define ansrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

src2srcml -t "a;\n" -l "C++"
check 3<<< "$ansrcml"

echo "a;" | src2srcml -l "C++"
check 3<<< "$ansrcml"

src2srcml -t "a;\n" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$ansrcml"

src2srcml -l C++ -t "a;\n" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$ansrcml"
