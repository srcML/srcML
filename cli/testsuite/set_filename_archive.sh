#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# filename flag on archive of single unit
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION" filename="compressed.tar.gz">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="-" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp "a;"

src2srcml sub/a.cpp --archive -f "compressed.tar.gz"
check 3<<< "$output"

src2srcml sub/a.cpp --archive --filename "compressed.tar.gz"
check 3<<< "$output"

src2srcml sub/a.cpp --archive --filename="compressed.tar.gz"
check 3<<< "$output"

src2srcml -l C++ --archive -f 'compressed.tar.gz' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$output"

src2srcml --archive  -f 'compressed.tar.gz' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output"


# standard input
echo -n "a;" | src2srcml -l C++ --archive -f compressed.tar.gz
check 3<<< "$output"

echo -n "a;" | src2srcml -l C++ --archive --filename compressed.tar.gz
check 3<<< "$output"

echo -n "a;" | src2srcml -l C++ --archive --filename=compressed.tar.gz
check 3<<< "$output"


# filename flag on archive of multiple units
define multi_unit_archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION" filename="compressed.tar.gz">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/b.cpp "b;"

src2srcml sub/a.cpp sub/b.cpp --in-order -f "compressed.tar.gz"
check 3<<< "$multi_unit_archive"

src2srcml sub/a.cpp sub/b.cpp --in-order -f=compressed.tar.gz
check 3<<< "$multi_unit_archive"

src2srcml --in-order -f "compressed.tar.gz" sub/a.cpp sub/b.cpp
check 3<<< "$multi_unit_archive"



