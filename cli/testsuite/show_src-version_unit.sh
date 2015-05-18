#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on single unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" directory="bar" filename="foo" version="1.0"/>
	STDOUT

# test on archive of one unit
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" version="1.0">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" version="1.0" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$input"
createfile sub/archive.cpp.xml "$archive"

srcml --show-src-version sub/a.cpp.xml
check 3<<< "1.0"

srcml --show-src-version < sub/a.cpp.xml
check 3<<< "1.0"

srcml --show-src-version sub/archive.cpp.xml
check 3<<< "1.0"

srcml --show-src-version < sub/archive.cpp.xml
check 3<<< "1.0"


# test src version on single unit with empty version
define empty <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="" directory="" filename="" version=""/>
	STDIN

# test on archive of one unit with an empty version
define emptyarchive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" version="">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" version="" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$empty"
createfile sub/archive.cpp.xml "$emptyarchive"

srcml --show-src-version sub/a.cpp.xml
check 3<<< ""

srcml --show-src-version < sub/a.cpp.xml
check 3<<< ""

srcml --show-src-version sub/archive.cpp.xml
check 3<<< ""

srcml --show-src-version < sub/archive.cpp.xml
check 3<<< ""


# test on empty archive with no version
define noneempty <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp"/>
	STDIN

# test on archive of one unit with no version
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDIN

createfile sub/a.cpp.xml "$noneempty"
createfile sub/archive.cpp.xml "$none"

srcml --show-src-version sub/a.cpp.xml
check_null

srcml --show-src-version < sub/a.cpp.xml
check_null

srcml --show-src-version sub/archive.cpp.xml
check_null

srcml --show-src-version < sub/archive.cpp.xml
check_null

