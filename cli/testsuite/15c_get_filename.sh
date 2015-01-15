#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get filename
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="bar" filename="foo" version="1.2"/>
	STDOUT

createfile sub/a.cpp.xml "$input"

srcml --get-filename sub/a.cpp.xml
check 3<<< "foo"

srcml --get-filename < sub/a.cpp.xml
check 3<<< "foo"

define empty <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="" dir="" filename="" version=""/>
	STDIN

createfile sub/a.cpp.xml "$empty"

srcml --get-filename sub/a.cpp.xml
check 3<<< ""

srcml --get-filename < sub/a.cpp.xml
check 3<<< ""

define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
	STDIN

createfile sub/a.cpp.xml "$none"

srcml --get-filename sub/a.cpp.xml
check_null

srcml --get-filename < sub/a.cpp.xml
check_null

