#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on standard in
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" directory="bar"/>
	STDOUT

# test on file
define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" directory="bar" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

echo -n "" | src2srcml -l C++ -d bar
check 3<<< "$output"

echo -n "" | src2srcml -l C++ --directory bar
check 3<<< "$output"

echo -n "" | src2srcml -l C++ --directory=bar
check 3<<< "$output"

src2srcml -d "bar" sub/a.cpp 
check 3<<< "$fsrcml"

src2srcml --directory "bar" sub/a.cpp 
check 3<<< "$fsrcml"

src2srcml --directory="bar" sub/a.cpp
check 3<<< "$fsrcml"

src2srcml -l C++ -d 'bar' -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml -d 'bar' sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml sub/a.cpp -d "bar"
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --directory "bar"
check 3<<< "$fsrcml"

src2srcml sub/a.cpp --directory="bar"
check 3<<< "$fsrcml"
