#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get filename
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="bar" filename="foo" version="1.2"/>
	STDOUT

createfile sub/a.cpp.xml "$input"

srcml --show-filename sub/a.cpp.xml
check 3<<< "foo"

srcml --show-filename < sub/a.cpp.xml
check 3<<< "foo"

define empty <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="" dir="" filename="" version=""/>
	STDIN

createfile sub/a.cpp.xml "$empty"

srcml --show-filename sub/a.cpp.xml
check 3<<< ""

srcml --show-filename < sub/a.cpp.xml
check 3<<< ""

define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
	STDIN

createfile sub/a.cpp.xml "$none"

srcml --show-filename sub/a.cpp.xml
check_null

srcml --show-filename < sub/a.cpp.xml
check_null

