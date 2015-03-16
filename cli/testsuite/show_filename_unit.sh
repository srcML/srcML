#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on individual unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" directory="bar" filename="foo" version="1.2"/>
	STDOUT

# test on archive of one
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" filename="a.cpp.tar">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/archive.cpp.xml "$archive"
createfile sub/a.cpp.xml "$input"

srcml --show-filename sub/a.cpp.xml
check 3<<< "foo"

srcml --show-filename < sub/a.cpp.xml
check 3<<< "foo"

srcml --show-filename sub/archive.cpp.xml
check 3<<< "a.cpp.tar"

srcml --show-filename < sub/archive.cpp.xml
check 3<<< "a.cpp.tar"


# test on unit with empty filename
define emptyunit <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="" directory="" filename="" version=""/>
	STDIN

# test on archive with empty filename
define emptyarchive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" filename=""/>
	STDOUT

createfile sub/a.cpp.xml "$emptyunit"
createfile sub/archive.cpp.xml "$emptyarchive"

srcml --show-filename sub/a.cpp.xml
check 3<<< ""

srcml --show-filename < sub/a.cpp.xml
check 3<<< ""

srcml --show-filename sub/archive.cpp.xml
check 3<<< ""

srcml --show-filename < sub/archive.cpp.xml
check 3<<< ""

# test on empty archive with no filename
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0"/>
	STDIN

createfile sub/archive.cpp.xml "$none"

srcml --show-filename sub/archive.cpp.xml
check_null

srcml --show-filename < sub/archive.cpp.xml
check_null

