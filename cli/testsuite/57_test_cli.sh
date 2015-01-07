#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test operator and modifier combinations

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" revision="0.8.0" language="C++"/>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

src2srcml -l C++ --operator --modifier < sub/a.cpp
check 3<<< "$output"

src2srcml --operator --modifier sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --operator --modifier -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$output"
rmfile sub/a.cpp.xml

src2srcml --operator --modifier sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"
rmfile sub/a.cpp.xml

src2srcml -l C++ --modifier --operator < sub/a.cpp
check 3<<< "$output"

src2srcml --modifier --operator sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --modifier --operator -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$output"
rmfile sub/a.cpp.xml

src2srcml --modifier --operator sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"
rmfile sub/a.cpp.xml

rmfile sub/a.cpp
