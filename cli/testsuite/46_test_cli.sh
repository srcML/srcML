#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++" filename="sub/a.cpp"/>
	INPUT


# separate
src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" "--

xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cppsrc2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" 
--xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" "--

xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xmlvalidate(open(sub/a.cpp.xml 'r').read() fsrcm)




# multc2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" 'sub/a.

src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" '

validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" 'sub/a.
cpp' -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)


src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cppsrc2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sfile1validate(open(sub/a.cpp.xml 'r').read() srcml)

src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xmlvalidate(open(sub/a.cpp.xml 'r').read() fsrcml)




src2srcml --debug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" 
'sub/a.c fsrcml)
src2srcm++ --debug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sfile1
src2srcmbug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xml
validate(open(sub/a.cpp.xml 'r').read() fsrcml)

