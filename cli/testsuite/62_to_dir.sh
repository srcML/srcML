#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" directory="sub" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" revision="0.8.0" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb" revision="0.8.0" language="C++" directory="sub" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDERR'
	    1 sub/a.cpp
	    2 sub/b.cpp
	STDERR

createfile sub/a.cpp.xml "$srcml"
rmfile sub/a.cpp

srcml2src --verbose --to-dir=. sub/a.cpp.xml

check sub/a.cpp 3<<< "a;" 4<<< "    1 sub/a.cpp"

createfile sub/a.cpp.xml "$nestedfile"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml2src --verbose --to-dir=. sub/a.cpp.xml
check sub/a.cpp 3<<< "a;" 4<<< "$output"
check sub/b.cpp 3<<< "b;"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml2src --verbose --to-dir '.' sub/a.cpp.xml

check sub/a.cpp 3<<< "a;" 4<<< "$output"
check sub/b.cpp 3<<< "b;"

