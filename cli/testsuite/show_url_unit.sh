#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get url on single unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="bar" filename="foo" version="1.2"/>
	STDOUT

# test on archive
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="bar">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"
createfile sub/archive.cpp.xml "$archive"

srcml --show-url sub/a.cpp.xml
check 3<<< "bar"

srcml --show-url < sub/a.cpp.xml
check 3<<< "bar"

srcml --show-url sub/archive.cpp.xml
check 3<<< "bar"

srcml --show-url < sub/archive.cpp.xml
check 3<<< "bar"


# empty on the unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="" url="" filename="" version=""/>
	STDOUT

# empty on the archive
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"
createfile sub/archive.cpp.xml "$empty"

srcml --show-url sub/a.cpp.xml
check 3<<< ""

srcml --show-url < sub/a.cpp.xml
check 3<<< ""

srcml --show-url sub/archive.cpp.xml
check 3<<< ""

srcml --show-url < sub/archive.cpp.xml
check 3<<< ""


# none
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
	STDIN

createfile sub/a.cpp.xml "$none"

srcml --show-url sub/a.cpp.xml
check_null

srcml --show-url < sub/a.cpp.xml
check_null

