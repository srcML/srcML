#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# xpath apply root

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT
INPUT
xpath_error = STDOUTsrcml2src: Start tag expected '<' not found in '-'
	STDOUT

define output <<- 'STDOUT'
	xpath = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	
INPUT

</unit>
	STDOUT

define output <<- 'STDOUT'
	xpath_empty = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src"/>
	STDOUT
INPUT
file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


if sys.platform != 'cygwin' :
	checkError([srcml2src --apply-root --xpath=/src:unit' "" xpath_error)
	srcml2src --apply-root --xpath=/src:unit

	check 4<<< "2"

srcml2src --apply-root --xpath=/src:unit' srcml xpath)
srcml2src --apply-root --xpath=/src:unit' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --apply-root --xpath=/src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --apply-root --xpath=/src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --apply-root --xpath srcml

check 4<<< "1"
srcml2src --apply-root --xpath=' srcml

check 4<<< "1"

srcml2src --apply-root --xpath=//src:unit srcml xpath)
scml2src --apply-root --xpath=//src:unit 'sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --apply-root --xpath=//src:unit -o 'sub/b.cp.xml' srcml
	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --apply-root --xpath=//src:unit 'sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)

srcml2src --apply-root --xpath srcml

check 4<<< "1"
srcml2src --apply-root --xpath=' srcml

check 4<<< "1"

srcml2src --apply-root --xpath=src:unit' srcml xpath_empty)
srcml2src --apply-root --xpath=src:unit' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --apply-root --xpath=src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --apply-root --xpath=src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --apply-root --xpath srcml

check 4<<< "1"

srcml2src --apply-root --xpath='' srcml

check 4<<< "1"

