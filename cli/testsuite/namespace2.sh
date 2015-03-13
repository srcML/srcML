#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT

echo -n "" | src2srcml -l C++ --xmlns:src="http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp"

check 3<<<"$output"

echo -n "" | src2srcml -l C++ --xmlns:src="http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp" -o sub/a.cpp.xml 

check sub/a.cpp.xml 3<<<"$output"

createfile sub/a.cpp ""

src2srcml --xmlns:src="http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<<"$foutput"

