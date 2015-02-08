#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# test
define foutput <<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
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
