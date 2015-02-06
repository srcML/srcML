#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++">
	</unit>
	STDOUT

define srcmlout <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="0.8.0" language="C++">
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# -X --no-namespace-decl
srcml2src -X --no-namespace-decl sub/a.cpp.xml 
check 3<<< "$srcmlout"

srcml2src -X --no-namespace-decl < sub/a.cpp.xml
check 3<<< "$srcmlout"

srcml2src -X --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcmlout"

srcml2src -X --no-namespace-decl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcmlout"

srcml2src -X --no-namespace-decl -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcmlout"

# --no-namespace-decl -X
srcml2src --no-namespace-decl -X sub/a.cpp.xml 
check 3<<< "$srcmlout"

srcml2src --no-namespace-decl -X < sub/a.cpp.xml
check 3<<< "$srcmlout"

srcml2src --no-namespace-decl -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcmlout"

srcml2src --no-namespace-decl -X -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcmlout"

srcml2src --no-namespace-decl -X -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcmlout"

# --no-namespace-decl
srcml2src --no-namespace-decl sub/a.cpp.xml
check 3<<< ""

srcml2src --no-namespace-decl < sub/a.cpp.xml
check 3<<< ""

