#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pos="http://www.srcML.org/srcML/position" revision="REVISION" language="C++" pos:tabs="8"/>
	STDOUT

xmlcheck "$fsrcml"
xmlcheck "$srcml"
createfile sub/a.cpp ""

# immediately following '--tabs' must be either a number or another option,
# or boost won't know whether the argument is an input file or argument
srcml --tabs sub/a.cpp
check_exit 1

srcml --tabs sub/a.cpp -o sub/a.cpp.xml
check_exit 1
