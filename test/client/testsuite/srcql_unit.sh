#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
#	<unit revision="REVISION" language="C++" filename="a.cpp" item="1" location="/src:expr_stmt[1]/src:expr[1]/src:name[1]"><name>a</name></unit>
define output1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" item="1"><name>a</name></unit>

	</unit>
	STDOUT

#	<unit revision="REVISION" language="C++" filename="a.cpp" item="1" location="/src:expr_stmt[1]/src:expr[1]/src:name[1]"><name>a</name></unit>
define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" item="1"><name>a</name></unit>

	</unit>
	STDOUT

define output_empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$output1"
xmlcheck "$output2"
xmlcheck "$output_empty"
xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

# /src:unit/src:expr_stmt/src:expr/src:name
srcml --xpath="/src:unit/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml
check "$output1"

srcml --xpath="/src:unit/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml --output-src
check "a"

srcml --xpath="/src:unit/src:expr_stmt/src:expr/src:name" < sub/a.cpp.xml
check "$output1"

srcml --xpath="/src:unit/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath="/src:unit/src:expr_stmt/src:expr/src:name" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath="/src:unit/src:expr_stmt/src:expr/src:name" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml
check "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml --output-src
check "a"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" < sub/a.cpp.xml
check "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output1"

srcml --xpath="/src:unit/src:expr_stmt" --xpath="/src:expr_stmt/src:expr/src:name" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output1"

# //src:name
srcml --xpath="//src:name" sub/a.cpp.xml
check "$output2"

srcml --xpath="//src:name" < sub/a.cpp.xml
check "$output2"

srcml --xpath="//src:name" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output2"

srcml --xpath="//src:name" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output2"

srcml --xpath="//src:name" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output2"

# src:name
srcml --xpath=name sub/a.cpp.xml
check "$output_empty"

srcml --xpath=name < sub/a.cpp.xml
check "$output_empty"

srcml --xpath=name sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$output_empty"

srcml --xpath=name -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$output_empty"

srcml --xpath=name -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$output_empty"

srcml --xpath="count(//src:name)" < sub/a.cpp.xml
check "1
"

define oneresult <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" item="1">a</unit>

	</unit>
	STDOUT

srcml --xpath="//src:name[1]/text()" < sub/a.cpp.xml
check "$oneresult"

srcml --xpath="boolean(//src:name)" < sub/a.cpp.xml
check "true
"

# test for omp
define ompsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:omp="http://www.srcML.org/srcML/openmp" revision="REVISION" language="C">
	<cpp:pragma>#<cpp:directive>pragma</cpp:directive> <omp:directive>omp <omp:name>parallel</omp:name></omp:directive></cpp:pragma>
	</unit>
	STDOUT

define xpathout <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:omp="http://www.srcML.org/srcML/openmp" revision="REVISION" language="C" item="1"><omp:directive>omp <omp:name>parallel</omp:name></omp:directive></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:omp="http://www.srcML.org/srcML/openmp" revision="REVISION" language="C" item="2"><omp:name>parallel</omp:name></unit>

	</unit>
	STDOUT

define xpathoutcpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:omp="http://www.srcML.org/srcML/openmp" revision="REVISION" language="C" item="1"><cpp:pragma>#<cpp:directive>pragma</cpp:directive> <omp:directive>omp <omp:name>parallel</omp:name></omp:directive></cpp:pragma></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" item="2"><cpp:directive>pragma</cpp:directive></unit>

	</unit>
	STDOUT

createfile ompsrcml.xml "$ompsrcml"

srcml --xpath="//omp:*" ompsrcml.xml
check "$xpathout"

srcml --xpath="//cpp:*" ompsrcml.xml
check "$xpathoutcpp"
