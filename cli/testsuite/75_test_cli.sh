#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src"/>
	STDOUT

createfile sub/a.cpp.xml $output

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' srcml xpath)
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --xpath srcml

check 4<<< "1"

srcml2src --xpath=''

check 4<<< "1"

srcml2src --xpath=//src:name' srcml xpath)
srcml2src --xpath=//src:name' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=//src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=//src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=src:name' srcml xpath_empty)
srcml2src --xpath=src:name' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --xpath=src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --xpath=src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

