#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define input <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	<!-- Comment Two -->
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	<!-- Comment Two -->
	</unit>

	</unit>
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp" item="1"><!-- Comment Two --></unit>

	</unit>
	STDOUT

srcml2src --xpath "//comment()" <<< "$input" 

check 3<<<"Comment One Comment Two"

srcml2src --xpath "string(//comment())" <<< "$input"

check 3<<<"Comment One Comment Two"

srcml2src --xpath "//comment()" <<< "$input"

srcml2src --xpath "string(//comment())" <<< "$input"

exit 0
# check srcml2src diff option

f = open('diff/diff.cpp.xml' 'r')
srcdiff = f.read()
f.close()

f = open('diff/a.cpp' 'r')
src_old = f.read()
f.close()

f = open('diff/a.cpp.xml' 'r')
