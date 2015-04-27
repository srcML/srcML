#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes">
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

src2srcml -t "ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding="ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml sub/a.cpp -t "ISO-8859-1"
check 3<<< "$foutput"

src2srcml sub/a.cpp --src-encoding "ISO-8859-1"
check 3<<< "$foutput"

src2srcml sub/a.cpp --src-encoding="ISO-8859-1"
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --src-encoding "ISO-8859-1" -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"
