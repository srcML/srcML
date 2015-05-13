#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define output <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:err="http://www.srcML.org/srcML/srcerr" revision="REVISION" language="C++"/>
	OUTPUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:err="http://www.srcML.org/srcML/srcerr" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

# test with --debug alone
echo -n "" | src2srcml -l C++ -g
check 3<<< "$output"

echo -n "" | src2srcml -l C++ --debug
check 3<<< "$output"

src2srcml --debug -l C++ < sub/a.cpp
check 3<<< "$output"

src2srcml --debug sub/a.cpp
check 3<<< "$foutput"

src2srcml sub/a.cpp --debug
check 3<<< "$foutput"

src2srcml -l C++ -g -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml -g sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

# test with src, cpp, and err namespaces
src2srcml --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" --xmlns:err="http://www.srcML.org/srcML/srcerr" sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" --xmlns:err="http://www.srcML.org/srcML/srcerr" -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" --xmlns:err="http://www.srcML.org/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:err="http://www.srcML.org/srcML/srcerr" sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:err="http://www.srcML.org/srcML/srcerr" -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.srcML.org/srcML/src" --xmlns:err="http://www.srcML.org/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns:cpp="http://www.srcML.org/srcML/cpp" --xmlns:err="http://www.srcML.org/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns:cpp="http://www.srcML.org/srcML/cpp" --xmlns:err="http://www.srcML.org/srcML/srcerr" -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"
