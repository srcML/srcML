#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test output options
define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

createfile sub/a.cpp "a;"

# output to scrML
src2srcml sub/a.cpp --output sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml sub/a.cpp --output=sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml -l C++ -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$sxmlfile"

src2srcml -l C++ - --output /dev/stdout < sub/a.cpp
check 3<<< "$xmlfile"

src2srcml -l C++ - -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$xmlfile"

# output to source code
define foutput <<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$foutput"

srcml2src sub/a.cpp.xml --output sub/a.cpp
check sub/a.cpp 3<<< "a;"

srcml2src sub/a.cpp.xml --output=sub/a.cpp
check sub/a.cpp 3<<< "a;"

srcml2src sub/a.cpp.xml -o sub/a.cpp
check sub/a.cpp 3<<< "a;"

srcml2src - -o sub/a.cpp <<< "$foutput"
check sub/a.cpp 3<<< "a;"

srcml2src -o sub/a.cpp <<< "$foutput"
check sub/a.cpp 3<<< "a;"

srcml2src - --output /dev/stdout <<< "$foutput"
check 3<<< "a;"

srcml2src - --output=/dev/stdout <<< "$foutput"
check 3<<< "a;"

srcml2src - -o /dev/stdout <<< "$foutput"
check 3<<< "a;"
