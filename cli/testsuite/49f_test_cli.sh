#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


# test metadata options with xml and unit

define nestedfileextra <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="Java" dir="emptysrc" filename="empty.java">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$nestedfileextra"

srcml2src --xml --unit "1" --get-language sub/a.cpp.xml
check 3<<< "C++"

srcml2src --xml --unit "1" --get-directory sub/a.cpp.xml
check 3<<< "sub"

srcml2src --xml --unit "1" --get-filename sub/a.cpp.xml
check 3<<< "a.cpp"

srcml2src --xml --unit "2" --get-language sub/a.cpp.xml
check 3<<< "Java"

srcml2src --xml --unit "2" --get-directory sub/a.cpp.xml
check 3<<< "emptysrc"

srcml2src --xml --unit "2" --get-filename sub/a.cpp.xml
check 3<<< "empty.java"

rmfile sub/a.cpp.xml
