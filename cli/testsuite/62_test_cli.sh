#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src"" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" revision="0.8.0" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

f = open(sub/a.cpp.xml 'w')
f.write(srcml)
f.close()


if platform.system() != "Windows" :
	os.system("rm -f sub/a.cpp")
else :
	os.system("del sub\\a.cpp")

#remove
srcml2sr --to-dir=. sub/a.cpp.xml

check sub/a.cpp <<< "a;"

createfile sub/a.cpp.xml <<< "$nestedfile"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml2src --to-dir=sub/a.cpp.xml

check sub/a.cpp 3<<< "a;"
check sub/b.cpp 3<<< "b;"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml2src -a '.' sub/a.cpp.xml

check sub/a.cpp 3<<< "a;"
check sub/b.cpp 3<<< "b;"

