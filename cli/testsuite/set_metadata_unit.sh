#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test metadata options with files
define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="sub" filename="a.cpp" version="1.2">
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.srcML.org/srcML/src">

	<src:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	<src:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	</src:unit>
	STDOUT

createfile sub/a.cpp.xml "$sxmlfile1"

srcml sub/a.cpp.xml --show-language
check 3<<< "C++"

srcml sub/a.cpp.xml --show-url
check 3<<< "sub"

srcml sub/a.cpp.xml --show-filename
check 3<<< "a.cpp"

srcml sub/a.cpp.xml --show-src-version
check 3<<< "1.2"

srcml sub/a.cpp.xml --show-encoding
check 3<<< "UTF-8"

srcml2src --units sub/a.cpp.xml
check 3<<< "1"

createfile sub/nested.cpp.xml "$nestedfile"

srcml2src --units sub/nested.cpp.xml
check 3<<< "2"
