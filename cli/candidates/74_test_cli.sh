#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" item="2"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>
	</unit>
	STDOUT

createfile sub/a.cpp.xml $output

srcml2src --apply-root --xpath=/src:unit #srcml_nested xpath_nested)

srcml2src --apply-root --xpath=/src:unit #sub/a.cpp.xml xpath_nested)

srcml2src --apply-root --xpath=/src:unit -o sub/b.cpp.xml #srcml_nested

readfile input sub/b.cpp.xml

srcml2src --apply-root --xpath=/src:unit #' sub/a.cpp.xml -o sub/b.cpp.xml ""

readfile input sub/b.cpp.xml

srcml2src --apply-root --xpath=//src:unit srcml_nested xath_nested_recursive)
srcml2src --apply-root --xpath=//src:unit 'sub/a.cpp.xml xpath_nested_recursive)
srcml2src --apply-root --xpath=//src:unit -o 'sub/b.cp.xml' srcml_nested
validate(open(sub/b.cpp.xml 'r').read() xpath_nested_recursive)
srcml2src --apply-root --xpath=//src:unit 'sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_nested_recursive)

srcml2src --apply-root --xpath=/src:unit/src:unit[1/src:expr_stmt' srcml_nested xpath_single_expr_stmt)
srcml2src --apply-root --xpath=/src:unit/src:unit[1/src:expr_stmt' sub/a.cpp.xml xpath_single_expr_stmt)

srcml2src --apply-root --xpath=/src:unit/src:unit[1/src:expr_stmt' -o sub/b.cpp.xml srcml_nested

validate(open(sub/b.cpp.xml 'r').read() xpath_single_expr_stmt)
srcml2src --apply-root --xpath=/src:unit/src:unit[1/src:expr_stmt' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_single_expr_stmt)


srcml2src --apply-root --xpath=//src:expr_stmt' srcml_nested xpath_nested_expr_stmt)
srcml2src --apply-root --xpath=//src:expr_stmt' sub/a.cpp.xml xpath_nested_expr_stmt)
srcml2src --apply-root --xpath=//src:expr_stmt' -o sub/b.cpp.xml srcml_nested

validate(open(sub/b.cpp.xml 'r').read() xpath_nested_expr_stmt)
srcml2src --apply-root --xpath=//src:expr_stmt' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_nested_expr_stmt)


