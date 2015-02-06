#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# no namespace declaration

define defaultxml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT

define nonamespacexml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="0.8.0" language="C++"/>
	STDOUT

echo -n "" | srcml2src -l C++

check 3<<< "$defaultxml"

echo -n "" | srcml2src -l C++ --no-namespace-decl

check 3<<< "$nonamespacexml"

# TODO: Split and put rest in xml_namespace test (they are testing removing namespace from xml)
createfile sub/a.cpp.xml "$defaultxml"

srcml2src -l C++ -X --no-namespace-decl < sub/a.cpp.xml
check 3<<< "$nonamespacexml"

srcml2src -X --no-namespace-decl sub/a.cpp.xml
check 3<<< "$nonamespacexml"

srcml2src -X --no-namespace-decl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nonamespacexml"

srcml2src -X --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml 
check sub/b.cpp.xml 3<<< "$nonamespacexml"

srcml2src -l C++ --no-namespace-decl -X < sub/a.cpp.xml
check 3<<< "$nonamespacexml"

srcml2src --no-namespace-decl -X sub/a.cpp.xml
check 3<<< "$nonamespacexml"

srcml2src --no-namespace-decl -X -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nonamespacexml"

srcml2src --no-namespace-decl -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$nonamespacexml"

srcml2src -l C++ --no-namespace-decl < sub/a.cpp.xml
check 3<<< "$nonamespacexml"

srcml2src --no-namespace-decl sub/a.cpp.xml
check 3<<< "$output"

srcml2src --no-namespace-decl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nonamespacexml"

srcml2src --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$nonamespacexml"
