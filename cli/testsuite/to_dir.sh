#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb" revision="REVISION" language="C++" filename="sub/b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDERR'
	    1 sub/a.cpp
	    2 sub/b.cpp
	STDERR

createfile a.cpp.xml "$srcml"
rmfile sub/a.cpp

srcml2src --verbose --to-dir=. a.cpp.xml

check sub/a.cpp 3<<< "a;" 4<<< "    1 sub/a.cpp"

createfile a.cpp.xml "$nestedfile"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml2src --verbose --to-dir=. a.cpp.xml
check sub/a.cpp 3<<< "a;" 4<<< "$output"
check sub/b.cpp 3<<< "b;"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml2src --verbose --to-dir '.' a.cpp.xml

check sub/a.cpp 3<<< "a;" 4<<< "$output"
check sub/b.cpp 3<<< "b;"

