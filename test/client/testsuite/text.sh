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
check "$srcml"

src2srcml --text "" -l "C++"
check "$srcml"

src2srcml --text="" -l "C++"
check "$srcml"

set +e

src2srcml --text -l "C++"
check_exit 7

set -e

echo -n "" | src2srcml -l "C++"
check "$srcml"

src2srcml -t "" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

src2srcml -l C++ -t "" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

src2srcml --text "" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

src2srcml -l C++ --text "" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

src2srcml --text="" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

src2srcml -l C++ --text="" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

set +e

src2srcml --text -l "C++" -o sub/a.cpp.xml
check_exit 7

src2srcml -l C++ --text -o sub/a.cpp.xml
check_exit 7

set -e

# simple input
define asrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$asrcml"

message "FIXME: Why is this message required?"

src2srcml -t "a;" -l "C++"
check "$asrcml"

src2srcml --text "a;" -l "C++"
check "$asrcml"

src2srcml --text="a;" -l "C++"
check "$asrcml"

echo -n "a;" | src2srcml -l "C++"
check "$asrcml"

src2srcml -t "a;" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

src2srcml -l C++ -t "a;" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

src2srcml --text "a;" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

src2srcml -l C++ --text "a;" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

src2srcml --text="a;" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

src2srcml -l C++ --text="a;" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

# escaped newline
define ansrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$ansrcml"

src2srcml -t "a;\n" -l "C++"
check "$ansrcml"

src2srcml --text "a;\n" -l "C++"
check "$ansrcml"

src2srcml --text="a;\n" -l "C++"
check "$ansrcml"

echo "a;" | src2srcml -l "C++"
check "$ansrcml"

src2srcml -t "a;\n" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

src2srcml -l C++ -t "a;\n" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

src2srcml --text "a;\n" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

src2srcml -l C++ --text "a;\n" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

src2srcml --text="a;\n" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

src2srcml -l C++ --text="a;\n" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

##
# Series of tests for --text escape. The full set of echo escape should be supported
# so tests are compared to output of echo
# Note that BSD echo (macOS) supports the -e escape, but does not have it listed
# on the man page

# escaped \a
text="\aa;\a"
src2srcml -l C++ --text="\aa;\a" | srcml2src
check "$(echo -en "\aa;\a")"

# escaped \b
text="\bb;\b"
src2srcml -l C++ --text="\bb;\b" | srcml2src
check "$(echo -en "\bb;\b")"

# escaped \f
src2srcml -l C++ --text="\ff;\f" | srcml2src
check "$(echo -en "\ff;\f")"

# escaped \t
src2srcml -l C++ --text="\tt;\t" | srcml2src
check "$(echo -en "\tt;\t")"

# escaped \v
src2srcml -l C++ --text="\vv;\v" | srcml2src
check "$(echo -en "\vv;\v")"

# hex characters
src2srcml -l C++ --text="\x68;" | srcml2src
check "$(echo -en "\x68;")"

src2srcml -l C++ --text="\x3;" | srcml2src
check "$(echo -en "\x3;")"

# octal characters
src2srcml -l C++ --text="\0150;" | srcml2src
check "$(echo -en "\0150;")"

src2srcml -l C++ --text="\03;" | srcml2src
check "$(echo -en "\03;")"

exit 0

# Problems with these

# escaped \r
# Note: \r currently get normalized to \n in src->srcml
text="\rr;\r"
src2srcml -l C++ --text="\rt;\r" | srcml2src
check $(echo -en "\nr;\n\n")

# escaped \e
src2srcml -l C++ --text="\ee;\e" | srcml2src
check "$(echo -en "\ee;\e")"
