#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test verbose on an archive
define sfile <<< "a;"

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define foutput <<- 'STDERR'
	XML encoding:  UTF-8
	    1 C++      1 a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8 sub/a.cpp

	Source Files: 1	Other Files: 0	Errors: 0	Total Files: 1
	STDERR

xmlcheck "$fsrcml"

createfile sub/a.cpp "a;"
createfile sub/a.cpp.xml "$fsrcml"

# from a file
srcml sub/a.cpp --archive --verbose
check "$fsrcml" "$foutput"

srcml --verbose sub/a.cpp --archive
check "$fsrcml" "$foutput"

srcml --verbose sub/a.cpp --archive -o sub/c.cpp.xml
check sub/c.cpp.xml "$fsrcml" "$foutput"

# standard in
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define output <<- 'STDERR'
	XML encoding:  UTF-8
	    1 C++      1 a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8

	Source Files: 1	Other Files: 0	Errors: 0	Total Files: 1
	STDERR

xmlcheck "$srcml"

srcml --verbose -l C++ --archive < sub/a.cpp
check "$srcml" "$output"

srcml --verbose -l C++ --archive -o sub/c.cpp.xml < sub/a.cpp
check sub/c.cpp.xml "$srcml" "$output"
