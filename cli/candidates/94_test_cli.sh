#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	srcml_nested = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT
<!-- Comment One -->
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
<!-- Comment Two -->
</unit>

</unit>
	STDOUT

xpath_comment = "//comment()"

xpath_comment_string = "string(//comment())"

define output <<- 'STDOUT'
	xpath_comment_output = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT

</unit>
	STDOUT

xpath_comment_string_output = STDOUT Comment 
	STDOUT

define output <<- 'STDOUT'
	xpath_comment_nested_output = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp" item="1"><!-- Comment Two --></unit>

</unit>
	STDOUT

xpath_comment_string_nested_output = STDOUT Comment One 
 Comment Two 
	STDOUT

srcml2src --xpath xpath_comment srcml xpath_comment_output)
srcml2src --xpath xpath_comment_string srcml xpath_comment_string_output)

srcml2src --xpath xpath_comment srcml_nested xpath_comment_nested_output)
srcml2src --xpath xpath_comment_string srcml_nested xpath_comment_string_nested_output)

# check srcml2src diff option

f = open('diff/diff.cpp.xml' 'r')
srcdiff = f.read()
f.close()

f = open('diff/a.cpp' 'r')
src_old = f.read()
f.close()

f = open('diff/a.cpp.xml' 'r')
