#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test no namespace decl
## input C++ to unit
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="REVISION" language="C++"/>
	STDOUT

echo -n "" | src2srcml -l C++ --no-namespace-decl
check 3<<< "$output"

define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT


createfile sub/a.cpp ""

src2srcml sub/a.cpp --no-namespace-decl -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output2"

src2srcml --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output2"

src2srcml --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output2"


## input xml to unit
define defaultxml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define nonamespacexml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="REVISION" language="C++"/>
	STDOUT

echo -n "" | srcml2src -l C++
check 3<<< "$defaultxml"

echo -n "" | srcml2src -l C++ --no-namespace-decl
check 3<<< "$nonamespacexml"

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

srcml2src -X -l C++ --no-namespace-decl < sub/a.cpp.xml
check 3<<< "$nonamespacexml"

srcml2src --no-namespace-decl -X sub/a.cpp.xml
check 3<<< "$nonamespacexml"

srcml2src --no-namespace-decl -o sub/b.cpp.xml -X sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nonamespacexml"

srcml2src --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml -X
check sub/b.cpp.xml 3<<< "$nonamespacexml"
