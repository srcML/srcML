#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT


createfile sub/a.cpp ""

# separate
src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp
check 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --debug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"
