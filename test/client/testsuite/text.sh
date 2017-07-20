#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# text flag with empty input
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

# requires a GNU echo, for macOS, gecho
if [[ "$(/bin/echo --version)" == "--version" ]]; then
	export ECHO=/usr/local/bin/gecho
else
	export ECHO=/bin/echo
fi

xmlcheck "$srcml"

srcml -t "" -l "C++"
check "$srcml"

srcml --text "" -l "C++"
check "$srcml"

srcml --text="" -l "C++"
check "$srcml"

srcml --text -l "C++"
check_exit 7

echo -n "" | srcml -l "C++"
check "$srcml"

mkdir sub

srcml -t "" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

srcml -l C++ -t "" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

srcml --text "" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

srcml -l C++ --text "" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

srcml --text="" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

srcml -l C++ --text="" -o sub/a.cpp.xml
check sub/a.cpp.xml "$srcml"

srcml --text -l "C++" -o sub/a.cpp.xml
check_exit 7

srcml -l C++ --text -o sub/a.cpp.xml
check_exit 7

# simple input
define asrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$asrcml"

message "FIXME: Why is this message required?"

srcml -t "a;" -l "C++"
check "$asrcml"

srcml --text "a;" -l "C++"
check "$asrcml"

srcml --text="a;" -l "C++"
check "$asrcml"

echo -n "a;" | srcml -l "C++"
check "$asrcml"

srcml -t "a;" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

srcml -l C++ -t "a;" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

srcml --text "a;" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

srcml -l C++ --text "a;" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

srcml --text="a;" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

srcml -l C++ --text="a;" -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

# escaped newline
define ansrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$ansrcml"

srcml -t "a;\n" -l "C++"
check "$ansrcml"

srcml --text "a;\n" -l "C++"
check "$ansrcml"

srcml --text="a;\n" -l "C++"
check "$ansrcml"

echo "a;" | srcml -l "C++"
check "$ansrcml"

srcml -t "a;\n" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

srcml -l C++ -t "a;\n" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

srcml --text "a;\n" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

srcml -l C++ --text "a;\n" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

srcml --text="a;\n" -l "C++" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

srcml -l C++ --text="a;\n" -o sub/a.cpp.xml
check sub/a.cpp.xml "$ansrcml"

##
# Series of tests for --text escape. The full set of echo escape should be supported
# so tests are compared to output of echo
# Note that BSD echo (macOS) supports the -e escape, but does not have it listed
# on the man page

# escaped \a
text="\aa;\a"
srcml -l C++ --text="\aa;\a" | srcml
message "$($ECHO -en "\aa;\a")"

# escaped \b
text="\bb;\b"
srcml -l C++ --text="\bb;\b" | srcml
check "$($ECHO -en "\bb;\b")"

# escaped \f
srcml -l C++ --text="\ff;\f" | srcml
check "$($ECHO -en "\ff;\f")"

# escaped \t
srcml -l C++ --text="\tt;\t" | srcml
check "$($ECHO -en "\tt;\t")"

# escaped \v
srcml -l C++ --text="\vv;\v" | srcml
check "$($ECHO -en "\vv;\v")"

# hex characters
srcml -l C++ --text="\x68;" | srcml
check "$($ECHO -en "\x68;")"

srcml -l C++ --text="\10;" | srcml
check "$($ECHO -en "\10;")"

# octal characters
srcml -l C++ --text="\0150;" | srcml
check "$($ECHO -en "\0150;")"

srcml -l C++ --text="\03;" | srcml
check "$($ECHO -en "\03;")"

# escaped \e
srcml -l C++ --text="\ee;\e" | srcml
check "$($ECHO -en "\ee;\e")"

exit 0

# escaped \r
# Note: \r currently get normalized to \n in src->srcml
# FIXME
srcml -l C++ --text="\rr;" | srcml
check $($ECHO -en "\nr;")
