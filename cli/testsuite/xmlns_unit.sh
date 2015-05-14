#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

# separate
src2srcml --xmlns="http://www.srcML.org/srcML/src" sub/a.cpp
check 3<<< "$foutput"

echo -n "" | src2srcml -l C++ --xmlns="http://www.srcML.org/srcML/src"
check 3<<< "$output"

echo -n "" | src2srcml -l C++ --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check 3<<< "$output"

echo -n "" | src2srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output"

src2srcml --xmlns="http://www.srcML.org/srcML/src" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check 3<<< "$foutput"

echo -n "" | src2srcml -l C++ --xmlns:cpp="http://www.srcML.org/srcML/cpp" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output"

src2srcml --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

# multiple
echo -n "" | src2srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check 3<<< "$output"

echo -n "" | src2srcml --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check 3<<< "$foutput"

echo -n "" | src2srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output"

src2srcml --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"
