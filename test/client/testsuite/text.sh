#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# text flag with empty input
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

xmlcheck "$srcml"

src2srcml -t "" -l "C++"
checkv2 "$srcml"

src2srcml --text "" -l "C++"
checkv2 "$srcml"

src2srcml --text="" -l "C++"
checkv2 "$srcml"

set +e

src2srcml --text -l "C++"
check_exit 7

set -e

echo -n "" | src2srcml -l "C++"
checkv2 "$srcml"

src2srcml -t "" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$srcml"

src2srcml -l C++ -t "" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$srcml"

src2srcml --text "" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$srcml"

src2srcml -l C++ --text "" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$srcml"

src2srcml --text="" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$srcml"

src2srcml -l C++ --text="" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$srcml"

set +e

src2srcml --text -l "C++" -o sub/a.cpp.xml
checkv2_exit 7

src2srcml -l C++ --text -o sub/a.cpp.xml
checkv2_exit 7

set -e

# simple input
define asrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$asrcml"

message "FIXME: Why is this message required?"

src2srcml -t "a;" -l "C++"
checkv2 "$asrcml"

src2srcml --text "a;" -l "C++"
checkv2 "$asrcml"

src2srcml --text="a;" -l "C++"
checkv2 "$asrcml"

echo -n "a;" | src2srcml -l "C++"
checkv2 "$asrcml"

src2srcml -t "a;" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$asrcml"

src2srcml -l C++ -t "a;" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$asrcml"

src2srcml --text "a;" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$asrcml"

src2srcml -l C++ --text "a;" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$asrcml"

src2srcml --text="a;" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$asrcml"

src2srcml -l C++ --text="a;" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$asrcml"

# embedded newline
define ansrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$ansrcml"

src2srcml -t "a;\n" -l "C++"
checkv2 "$ansrcml"

src2srcml --text "a;\n" -l "C++"
checkv2 "$ansrcml"

src2srcml --text="a;\n" -l "C++"
checkv2 "$ansrcml"

echo "a;" | src2srcml -l "C++"
checkv2 "$ansrcml"

src2srcml -t "a;\n" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$ansrcml"

src2srcml -l C++ -t "a;\n" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$ansrcml"

src2srcml --text "a;\n" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$ansrcml"

src2srcml -l C++ --text "a;\n" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$ansrcml"

src2srcml --text="a;\n" -l "C++" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$ansrcml"

src2srcml -l C++ --text="a;\n" -o sub/a.cpp.xml
checkv2 sub/a.cpp.xml "$ansrcml"
