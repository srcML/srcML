#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on individual unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
  STDOUT

xmlcheck "$input"
createfile sub/a.cpp.xml "$input"
createfile sub/a.cpp "a;"

srcml sub/a.cpp.xml --show-filename
check "sub/a.cpp"

srcml --show-filename sub/a.cpp.xml
check "sub/a.cpp"

srcml --show-filename < sub/a.cpp.xml
check "sub/a.cpp"

srcml sub/a.cpp --show-filename
check "sub/a.cpp"

srcml --show-filename sub/a.cpp
check "sub/a.cpp"

