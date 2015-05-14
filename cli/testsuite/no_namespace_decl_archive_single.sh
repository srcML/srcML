#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test no namespace decl
## input C++ to unit
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT


createfile sub/a.cpp "a;"

src2srcml sub/a.cpp --no-namespace-decl --archive
check 3<<< "$srcml"

src2srcml sub/a.cpp --archive --no-namespace-decl
check 3<<< "$srcml"

src2srcml --archive sub/a.cpp --no-namespace-decl
check 3<<< "$srcml"

src2srcml --archive --no-namespace-decl sub/a.cpp
check 3<<< "$srcml"

src2srcml sub/a.cpp --no-namespace-decl -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml sub/a.cpp --no-namespace-decl --archive -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml sub/a.cpp --archive --no-namespace-decl -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --archive sub/a.cpp --no-namespace-decl -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --no-namespace-decl sub/a.cpp --archive -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --no-namespace-decl --archive sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --archive --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --no-namespace-decl sub/a.cpp --archive -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --no-namespace-decl --archive sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"

src2srcml --archive --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$srcml"


## input xml to unit
define defaultxml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$defaultxml"

srcml2src -l C++ -X --no-namespace-decl < sub/a.cpp.xml
check 3<<< "$srcml"

srcml2src -X --no-namespace-decl sub/a.cpp.xml
check 3<<< "$srcml"

srcml2src -X --no-namespace-decl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -X --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml 
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -l C++ --no-namespace-decl -X < sub/a.cpp.xml
check 3<<< "$srcml"

srcml2src --no-namespace-decl -X sub/a.cpp.xml
check 3<<< "$srcml"

srcml2src --no-namespace-decl -X -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src --no-namespace-decl -X sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src -X -l C++ --no-namespace-decl < sub/a.cpp.xml
check 3<<< "$srcml"

srcml2src --no-namespace-decl -X sub/a.cpp.xml
check 3<<< "$srcml"

srcml2src --no-namespace-decl -o sub/b.cpp.xml -X sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml2src --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml -X
check sub/b.cpp.xml 3<<< "$srcml"
