#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srca <<- 'STDOUT'
	a;
	STDOUT

define srcb <<- 'STDOUT'
	b;
	STDOUT

# multi archive
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

# srcml2src - input srcml single file
createfile sub/a.cpp.xml "$srcml"

srcml2src --output-src sub/a.cpp.xml
check 3<<< "$srca"

srcml2src -S sub/a.cpp.xml
check 3<<< "$srca"

srcml2src -U 1 --output-src sub/a.cpp.xml
check 3<<< "$srca"

srcml2src -U 1 -S sub/a.cpp.xml
check 3<<< "$srca"

srcml2src -U 2 --output-src sub/a.cpp.xml
check 3<<< "$srcb"

srcml2src -U 2 -S sub/a.cpp.xml
check 3<<< "$srcb"

src2srcml -S < sub/a.cpp.xml
check 3<<< "$srca"

src2srcml -U 1 -S < sub/a.cpp.xml
check 3<<< "$srca"

src2srcml -U 2 -S < sub/a.cpp.xml
check 3<<< "$srcb"

src2srcml -S sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp 3<<< "$srca"

src2srcml -S -U 1 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp 3<<< "$srca"

src2srcml -S -U 2 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp 3<<< "$srcb"

src2srcml -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -S -U 1 -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -U 1 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -U 2 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp 3<<< "$srcb"

src2srcml -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -U 1 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -U 2 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp 3<<< "$srcb"

# single archive
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

# srcml2src - input srcml single file
createfile sub/a.cpp.xml "$srcml"

srcml2src --output-src sub/a.cpp.xml
check 3<<< "$srca"

srcml2src -S sub/a.cpp.xml
check 3<<< "$srca"

srcml2src -U 1 --output-src sub/a.cpp.xml
check 3<<< "$srca"

srcml2src -U 1 -S sub/a.cpp.xml
check 3<<< "$srca"

src2srcml -S < sub/a.cpp.xml
check 3<<< "$srca"

src2srcml -U 1 -S < sub/a.cpp.xml
check 3<<< "$srca"

src2srcml -S sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp 3<<< "$srca"

src2srcml -S -U 1 sub/a.cpp.xml -o sub/b.cpp
check sub/b.cpp 3<<< "$srca"

src2srcml -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -S -U 1 -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -U 1 -S -o sub/b.cpp sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

src2srcml -U 1 -S -o sub/b.cpp < sub/a.cpp.xml
check sub/b.cpp 3<<< "$srca"

