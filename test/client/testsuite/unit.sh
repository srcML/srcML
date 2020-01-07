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
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define sxmlfile2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$sxmlfile1"
xmlcheck "$sxmlfile2"
xmlcheck "$nestedfile"
createfile sub/a.cpp.xml "$nestedfile"

srcml sub/a.cpp.xml -U "1"
check "$sfile1"

srcml sub/a.cpp.xml --unit "1"
check "$sfile1"

srcml sub/a.cpp.xml --unit="1"
check "$sfile1"

srcml --unit "1" -o sub/a.cpp < sub/a.cpp.xml
check sub/a.cpp "$sfile1"

srcml --unit "1" sub/a.cpp.xml -o sub/a.cpp
check sub/a.cpp "$sfile1"

srcml sub/a.cpp.xml -U "2"
check "$sfile2"

srcml sub/a.cpp.xml --unit "2"
check "$sfile2"

srcml sub/a.cpp.xml --unit="2"
check "$sfile2"

srcml --unit "2" -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp "$sfile2"

srcml --unit "2" sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp "$sfile2"

# check xml and unit option
srcml -X --unit "1" sub/a.cpp.xml
check "$sxmlfile1"

srcml -X --unit "1" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$sxmlfile1"

srcml -X --unit "1" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$sxmlfile1"

srcml -X --unit "2" sub/a.cpp.xml
check "$sxmlfile2"

srcml -X --unit "2" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$sxmlfile2"

srcml -X --unit "2" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$sxmlfile2"
