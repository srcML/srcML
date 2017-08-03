#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# empty with debug
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="Java"/>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="Java" filename="sub/a.java"/>
	STDOUT

xmlcheck "$output"
xmlcheck "$foutput"

createfile sub/a.java ""

srcml --cpp sub/a.java
check "$foutput"

echo -n "" | srcml -l Java --cpp -o sub/a.java.xml
check sub/a.java.xml "$output"

srcml --cpp sub/a.java -o sub/a.java.xml
check sub/a.java.xml "$foutput"
