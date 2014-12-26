#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	srcml_nested = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	
INPUT
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
	STDOUT

xpath_attribute = "//src:unit/@filename"

xpath_attribute_string = "string(//src:unit/@filename)"

define output <<- 'STDOUT'
	xpath_attribute_output = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	
INPUT

</unit>
	STDOUT

xpath_attribute_string_output = STDOUTa.cpp
	STDOUT

define output <<- 'STDOUT'
	xpath_attribute_nested_output = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	
INPUT

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="b.cpp" item="1">b.cpp</unit>

</unit>
	STDOUT

xpath_attribute_string_nested_output = STDOUTa.cpp
b.cpp
	STDOUT

srcml2src --xpath xpath_attribute srcml xpath_attribute_output)
srcml2src --xpath xpath_attribute_string srcml xpath_attribute_string_output)

srcml2src --xpath xpath_attribute srcml_nested xpath_attribute_nested_output)
srcml2src --xpath xpath_attribute_string srcml_nested xpath_attribute_string_nested_output)

# comment

