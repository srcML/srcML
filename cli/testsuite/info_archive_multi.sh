#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# srcML info and longinfo

define info <<- 'STDOUT'
	xmlns="http://www.srcML.org/srcML/src"
	encoding="UTF-8"
	url="test"
	STDOUT

define longinfo <<- 'STDOUT'
	xmlns="http://www.srcML.org/srcML/src"
	encoding="UTF-8"
	url="test"
	units="2"
	STDOUT


# test on archive of many units
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/archive_multi.xml "$srcml"

srcml sub/archive_multi.xml -i
check 3<<< "$info"

srcml sub/archive_multi.xml --info
check 3<<< "$info"

srcml --info < sub/archive_multi.xml
check 3<<< "$info"

srcml -i sub/archive_multi.xml
check 3<<< "$info"

srcml --info sub/archive_multi.xml
check 3<<< "$info"

srcml --longinfo sub/archive_multi.xml
check 3<<< "$longinfo"

srcml sub/archive_multi.xml --longinfo
check 3<<< "$longinfo"

srcml --longinfo < sub/archive_multi.xml
check 3<<< "$longinfo"


# test on archive of one unit
define longinfo <<- 'STDOUT'
	xmlns="http://www.srcML.org/srcML/src"
	encoding="UTF-8"
	url="test"
	units="1"
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/archive_single.xml "$srcml"

srcml sub/archive_single.xml -i
check 3<<< "$info"

srcml sub/archive_single.xml --info
check 3<<< "$info"

srcml --info < sub/archive_single.xml
check 3<<< "$info"

srcml -i sub/archive_single.xml
check 3<<< "$info"

srcml --info sub/archive_single.xml
check 3<<< "$info"

srcml --longinfo sub/archive_single.xml
check 3<<< "$longinfo"

srcml sub/archive_single.xml --longinfo
check 3<<< "$longinfo"

srcml --longinfo < sub/archive_single.xml
check 3<<< "$longinfo"
