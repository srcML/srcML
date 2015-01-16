#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get directory
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="bar" filename="foo" version="1.2"/>
	STDOUT

createfile sub/a.cpp.xml "$input"

srcml --get-directory sub/a.cpp.xml
check 3<<< "bar"

srcml --get-directory < sub/a.cpp.xml
check 3<<< "bar"

# empty
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="" dir="" filename="" version=""/>
	STDOUT

createfile sub/a.cpp.xml "$input"

srcml --get-directory sub/a.cpp.xml
check 3<<< ""

srcml --get-directory < sub/a.cpp.xml
check 3<<< ""

# none
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
	STDIN

createfile sub/a.cpp.xml "$none"

srcml --get-directory sub/a.cpp.xml
check_null

srcml --get-directory < sub/a.cpp.xml
check_null

