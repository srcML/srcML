#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
INPUT
<foo:a/>
</unit>

<unit xmlns:bar="http://www.cs.uakron.edu/~collard/foo" revision="0.8.0" language="Java">
<foo:a/><bar:b/>
</unit>

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


srcml2src --xpath=/src:unit' srcml srcml)
echo -n "" | srcml2src --xpath=/src:unit' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xpath=/src:unit' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xpath=/src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=//src:unit srcml srcml)
scml2src --xpath=//src:unit 'sub/a.cpp.xml<<< ""
if sys.platform != 'cygwin' :
	srcml2src --xpath=//src:unit -o 'sub/b.cp.xml' srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xpath=//src:unit 'sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)

srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=src:unit' srcml xpath_empty)
srcml2src --xpath=src:unit' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --xpath=src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --xpath=src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

# xslt and param

