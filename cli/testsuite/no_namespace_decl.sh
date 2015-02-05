#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test no namespace decl

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="0.8.0" language="C++"/>
	STDOUT

echo -n "" | src2srcml -l C++ --no-namespace-decl

check 3<<< "$output"

define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT


createfile sub/a.cpp ""

src2srcml --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$output2"
