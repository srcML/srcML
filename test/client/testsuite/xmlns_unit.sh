#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
	STDOUT

define outputnocpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
	STDOUT

define foutputnocpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
	STDOUT

xmlcheck "$output"
xmlcheck "$foutput"
createfile sub/a.cpp "#define"

# separate
srcml --xmlns="http://www.srcML.org/srcML/src" sub/a.cpp
check "$foutputnocpp"

echo -n "#define" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src"
check "$outputnocpp"

echo -n "#define" | srcml -l C++ --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check "$output"

echo -n "#define" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" -o sub/a.cpp.xml
check sub/a.cpp.xml "$outputnocpp"

srcml --xmlns="http://www.srcML.org/srcML/src" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutputnocpp"

srcml --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check "$foutput"

echo -n "#define" | srcml -l C++ --xmlns:cpp="http://www.srcML.org/srcML/cpp" -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

srcml --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutput"

# multiple
echo -n "#define" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check "$output"

srcml --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check "$foutput"

echo -n "#define" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

srcml --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutput"
