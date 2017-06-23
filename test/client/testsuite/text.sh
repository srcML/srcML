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

# embedded newline
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

# embedded \a
define atsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><escape char="0x07"/><expr_stmt><expr><name>a</name></expr>;</expr_stmt><escape char="0x07"/></unit>
	STDOUT

src2srcml -l C++ --text="\aa;\a" -o sub/a.cpp.xml
check sub/a.cpp.xml "$atsrcml"

# embedded \b
define atsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><escape char="0x08"/><expr_stmt><expr><name>a</name></expr>;</expr_stmt><escape char="0x08"/></unit>
	STDOUT

src2srcml -l C++ --text="\ba;\b" -o sub/a.cpp.xml
check sub/a.cpp.xml "$atsrcml"

# embedded \f
define atsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><escape char="0x0c"/><expr_stmt><expr><name>a</name></expr>;</expr_stmt><escape char="0x0c"/></unit>
	STDOUT

src2srcml -l C++ --text="\fa;\f" -o sub/a.cpp.xml
check sub/a.cpp.xml "$atsrcml"

# embedded \r
define atsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

src2srcml -l C++ --text="\ra;\r" -o sub/a.cpp.xml
check sub/a.cpp.xml "$atsrcml"

# embedded \t
define atsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++">	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>	</unit>
	STDOUT

src2srcml -l C++ --text="\ta;\t" -o sub/a.cpp.xml
check sub/a.cpp.xml "$atsrcml"

# embedded \v
define atsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><escape char="0x0b"/><expr_stmt><expr><name>a</name></expr>;</expr_stmt><escape char="0x0b"/></unit>
	STDOUT

src2srcml -l C++ --text="\va;\v" -o sub/a.cpp.xml
check sub/a.cpp.xml "$atsrcml"

# embedded \e
define atsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><escape char="0x1b"/><expr_stmt><expr><name>a</name></expr>;</expr_stmt><escape char="0x1b"/></unit>
	STDOUT

src2srcml -l C++ --text="\ea;\e" -o sub/a.cpp.xml
check sub/a.cpp.xml "$atsrcml"







