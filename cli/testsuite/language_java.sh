#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

src2srcml -l "Java" sub/a.cpp
check 3<<< "$output"

src2srcml --language "Java" sub/a.cpp
check 3<<< "$output"

src2srcml --language="Java" sub/a.cpp
check 3<<< "$output"

src2srcml sub/a.cpp -l "Java"
check 3<<< "$output"

src2srcml sub/a.cpp --language "Java"
check 3<<< "$output"

src2srcml sub/a.cpp --language="Java"
check 3<<< "$output"

src2srcml -l 'Java' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$output"

src2srcml -l 'Java' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output"
