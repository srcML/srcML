#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# empty with debug
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java"/>
	STDOUT
INPUT
define output <<- 'STDOUT'
	f<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" filename="sub/a.java"/>
	INPUT
src2srcml sub/a.java' --cpp fsrcml)
src2srcml sub/a.java' --cpp fsrcml)
src2srcml sub/a.java' --cpp fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l 'Java' --cpp -o 'sub/a.java.xml' sfile1
	readfile input sub/a.java.xml' 'r').read() srcml)
src2srcml --cpp 'sub/a.java'-o 'sub/a.java.xml' ""
readfile input sub/a.java.xml' 'r').read() fsrcml)

