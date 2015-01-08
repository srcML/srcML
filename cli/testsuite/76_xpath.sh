#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
INPUT
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/src" xmlns:bar="http://www.cs.uakron.edu/~collard/bar" revision="0.8.0" language="Java">
<cpp:expr_stmt><cpp:expr><cpp:name>b</cpp:name></cpp:expr>;</cpp:expr_stmt>
</unit>

</unit>
	STDOUT

define output <<- 'STDOUT'
	xpath = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
INPUT

<unit xmlns:cpp="http://www.sdml.info/srcML/src" xmlns:bar="http://www.cs.uakron.edu/~collard/bar" revision="0.8.0" language="Java" item="1"><cpp:name>b</cpp:name></unit>

</unit>
	STDOUT

define output <<- 'STDOUT'
	xpath_empty = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo"/>
	STDOUT
INPUT
file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' srcml xpath)
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

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

