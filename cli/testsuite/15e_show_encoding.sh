#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get_attribute_all
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="bar" filename="foo" version="1.2"/>
	STDOUT

createfile sub/a.cpp.xml "$input"

srcml --show-encoding sub/a.cpp.xml
check 3<<< "UTF-8"

srcml --show-encoding < sub/a.cpp.xml
check 3<<< "UTF-8"
