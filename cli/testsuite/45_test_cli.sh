#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp/>
	INPUT

f = open(sub/a.cpp 'w')
f.write(sfile1)
f.close()

# separate
src2srcml --xmlns="http://www.sdml.info/srcML/src" sub/a.cpp
src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)


src2srcml --xmlns="http://www.sdml.info/srcML/src" sub/a.cpp -o sub/a.cpp.xml


validate(open(sub/a.cpp.xml 'r').read() fsrcml)

src2srcml --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp



src2srcml -l C++ --xmlns:cpp="http://www.sdml.info/srcML/cpp" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)


src2srcml --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)




# multiple
src2srcml --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp "" 

fsrcml)
src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" -o 'sub/a
.cpp.xml' sfile1

validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp -o 
sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)


