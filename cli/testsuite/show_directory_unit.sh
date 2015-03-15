#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get directory on single unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" directory="bar" filename="foo" version="1.2"/>
	STDOUT

# test on archive
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" directory="bar">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"
createfile sub/archive.cpp.xml "$archive"

srcml --show-directory sub/a.cpp.xml
check 3<<< "bar"

srcml --show-directory < sub/a.cpp.xml
check 3<<< "bar"

srcml --show-directory sub/archive.cpp.xml
check 3<<< "bar"

srcml --show-directory < sub/archive.cpp.xml
check 3<<< "bar"


# empty on the unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="" directory="" filename="" version=""/>
	STDOUT

# empty on the archive
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" directory="">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"
createfile sub/archive.cpp.xml "$empty"

srcml --show-directory sub/a.cpp.xml
check 3<<< ""

srcml --show-directory < sub/a.cpp.xml
check 3<<< ""

srcml --show-directory sub/archive.cpp.xml
check 3<<< ""

srcml --show-directory < sub/archive.cpp.xml
check 3<<< ""


# none
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0"/>
	STDIN

createfile sub/a.cpp.xml "$none"

srcml --show-directory sub/a.cpp.xml
check_null

srcml --show-directory < sub/a.cpp.xml
check_null

