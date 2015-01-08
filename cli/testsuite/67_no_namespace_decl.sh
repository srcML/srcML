#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++">
</unit>
	STDOUT

define output <<- 'STDOUT'
	srcmlout = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++">
	</unit>
INPUT

f = open(sub/a.cpp.xml 'w')
f.write(srcml)
f.close()


srcml2src --xml --no-namespace-decl srcml srcmlout)
srcml2src --xml --no-namespace-decl 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --xml --no-namespace-decl -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --xml --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


srcml2src --no-namespace-decl --xml srcml srcmlout)
srcml2src --no-namespace-decl --xml 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --no-namespace-decl --xml -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --no-namespace-decl --xml sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


srcml2src --no-namespace-decl srcml srcmlout)
srcml2src --no-namespace-decl 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --no-namespace-decl -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


