#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test unit option

define sfile1 <<- 'STDOUT'
	a;
	STDOUT

define sfile2 <<- 'STDOUT'
	b;
	STDOUT

define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define sxmlfile2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	
	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$nestedfile"

srcml2src sub/a.cpp.xml -U "1"
check 3<<< "$sfile1"

srcml2src sub/a.cpp.xml --unit "1"
check 3<<< "$sfile1"

srcml2src sub/a.cpp.xml --unit="1"
check 3<<< "$sfile1"

srcml2src --unit "1" -o sub/a.cpp < sub/a.cpp.xml
check sub/a.cpp 3<<< "$sfile1"
rmfile sub/a.cpp

srcml2src --unit "1" sub/a.cpp.xml -o sub/a.cpp
check sub/a.cpp 3<<< "$sfile1"
rmfile sub/a.cpp

srcml2src sub/a.cpp.xml -U "2" 
check 3<<< "$sfile2"

srcml2src sub/a.cpp.xml --unit "2"
check 3<<< "$sfile2"

srcml2src sub/a.cpp.xml --unit="2"
check 3<<< "$sfile2"

srcml2src --unit "2" -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp 3<<< "$sfile2"
rmfile sub/b.cpp

srcml2src --unit "2" sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp 3<<< "$sfile2"
rmfile sub/b.cpp


# check xml and unit option
srcml2src --xml --unit "1" sub/a.cpp.xml
check 3<<< "$sxmlfile1"

srcml2src --xml --unit "1" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$sxmlfile1"
rmfile sub/b.cpp.xml

srcml2src --xml --unit "1" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$sxmlfile1"
rmfile sub/b.cpp.xml

srcml2src --xml --unit "2" sub/a.cpp.xml 
check 3<<< "$sxmlfile2"

srcml2src --xml --unit "2" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$sxmlfile2"
rmfile sub/b.cpp.xml

srcml2src --xml --unit "2" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$sxmlfile2"
rmfile sub/b.cpp.xml

rmfile sub/a.cpp.xml
