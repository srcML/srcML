#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# nested file input
define nested <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a2.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b2.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define nested_unit_out <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a2.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b2.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define nested_exprstmt_out <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" item="1"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" item="1"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a2.cpp" item="1"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b2.cpp" item="1"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$nested"

# /src:unit
srcml2src --apply-root --xpath=/src:unit sub/a.cpp.xml
check 3<<< "$nested_unit_out"

srcml2src --apply-root --xpath=/src:unit < sub/a.cpp.xml
check 3<<< "$nested_unit_out"

srcml2src --apply-root --xpath=/src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_unit_out"

srcml2src --apply-root --xpath=/src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_unit_out"

srcml2src --apply-root --xpath=/src:unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_unit_out"

# //src:unit
srcml2src --apply-root --xpath=//src:unit sub/a.cpp.xml
check 3<<< "$nested_unit_out"

srcml2src --apply-root --xpath=//src:unit < sub/a.cpp.xml
check 3<<< "$nested_unit_out"

srcml2src --apply-root --xpath=//src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_unit_out"

srcml2src --apply-root --xpath=//src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_unit_out"

srcml2src --apply-root --xpath=//src:unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_unit_out"

# //src:expr_stmt
srcml2src --apply-root --xpath=//src:expr_stmt sub/a.cpp.xml
check 3<<< "$nested_exprstmt_out"

srcml2src --apply-root --xpath=//src:expr_stmt < sub/a.cpp.xml
check 3<<< "$nested_exprstmt_out"

srcml2src --apply-root --xpath=//src:expr_stmt sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_exprstmt_out"

srcml2src --apply-root --xpath=//src:expr_stmt -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_exprstmt_out"

srcml2src --apply-root --xpath=//src:expr_stmt -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$nested_exprstmt_out"

#
#srcml2src --apply-root --xpath=/src:unit/src:unit[1]/src:expr_stmt srcml_nested xpath_single_expr_stmt)
#
#srcml2src --apply-root --xpath=/src:unit/src:unit[1]/src:expr_stmt sub/a.cpp.xml xpath_single_expr_stmt)
#
#srcml2src --apply-root --xpath=/src:unit/src:unit[1]/src:expr_stmt -o sub/b.cpp.xml srcml_nested
#
#srcml2src --apply-root --xpath=/src:unit/src:unit[1]/src:expr_stmt sub/a.cpp.xml -o sub/b.cpp.xml ""
#
#

