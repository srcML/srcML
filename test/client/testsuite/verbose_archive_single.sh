#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test verbose on an archive
define sfile <<< "a;"

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define foutput <<- 'STDERR'
	Source encoding:  (null)
	XML encoding:  UTF-8
	    1 sub/a.cpp	C++	1	a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8

	Translated: 1	Skipped: 0	Error: 0	Total: 1
	STDERR

xmlcheck "$fsrcml"

createfile sub/a.cpp "a;"
createfile sub/a.cpp.xml "$fsrcml"
 
# from a file
srcml sub/a.cpp --archive --verbose
checkv2 "$fsrcml" 4<<< "$foutput"

srcml --verbose sub/a.cpp --archive
checkv2 "$fsrcml" 4<<< "$foutput"

srcml --verbose sub/a.cpp --archive -o sub/c.cpp.xml
checkv2 sub/c.cpp.xml "$fsrcml" 4<<< "$foutput"

# standard in
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define output <<- 'STDERR'
	Source encoding:  (null)
	XML encoding:  UTF-8
	    1 	C++	1	a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8

	Translated: 1	Skipped: 0	Error: 0	Total: 1
	STDERR

xmlcheck "$srcml"

srcml --verbose -l C++ --archive < sub/a.cpp
checkv2 "$srcml" 4<<< "$output"

srcml --verbose -l C++ --archive -o sub/c.cpp.xml < sub/a.cpp
checkv2 sub/c.cpp.xml "$srcml" 4<<< "$output"
