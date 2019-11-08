#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C#" filename="sub/a.cpp"/>
	STDOUT

xmlcheck "$output"
createfile sub/a.cpp ""

srcml -l "C#" sub/a.cpp
check "$output"

srcml --language "C#" sub/a.cpp
check "$output"

srcml --language="C#" sub/a.cpp
check "$output"

srcml sub/a.cpp -l "C#"
check "$output"

srcml sub/a.cpp --language "C#"
check "$output"

srcml sub/a.cpp --language="C#"
check "$output"

srcml -l 'C#' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml "$output"

srcml -l 'C#' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

