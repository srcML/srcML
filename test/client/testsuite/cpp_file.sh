#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# cpp

define input <<- 'INPUT'
	#if A
	break;
	#else
	return;
	#endif
    INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="Java"><cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
    </unit>
	STDOUT
xmlcheck "$output"

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="Java" filename="sub/a.java">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT
xmlcheck "$foutput"

createfile sub/a.java "$input"

# standard in
#srcml -l Java -cpp <<< "$input"
#check "$output"
#
#srcml -cpp -l Java <<< "$input"
#check "$output"
#
#srcml -l Java -cpp -o sub/a.java.xml <<< "$input"
#check sub/a.java.xml "$output"
#
#srcml -l Java -o sub/a.java.xml -cpp <<< "$input"
#check sub/a.java.xml "$output"
#
#srcml -o sub/a.java.xml -l Java -cpp <<< "$input"
#check sub/a.java.xml "$output"
#
#srcml -o sub/a.java.xml -cpp -l Java <<< "$input"
#check sub/a.java.xml "$output"
#
#srcml -cpp -o sub/a.java.xml -l Java <<< "$input"
#check sub/a.java.xml "$output"
#
#srcml -cpp -l Java -o sub/a.java.xml <<< "$input"
#check sub/a.java.xml "$output"

echo "$input" | srcml -l Java --cpp
check "$output"

echo "$input" | srcml --cpp -l Java
check "$output"

echo "$input" | srcml -l Java --cpp -o sub/a.java.xml
check sub/a.java.xml "$output"

echo "$input" | srcml -l Java -o sub/a.java.xml --cpp
check sub/a.java.xml "$output"

echo "$input" | srcml --cpp -l Java -o sub/a.java.xml
check sub/a.java.xml "$output"

echo "$input" | srcml --cpp -o sub/a.java.xml -l Java
check sub/a.java.xml "$output"

echo "$input" | srcml -o sub/a.java.xml -l Java --cpp
check sub/a.java.xml "$output"

echo "$input" | srcml -o sub/a.java.xml --cpp -l Java
check sub/a.java.xml "$output"

# from file - options after input
srcml sub/a.java -l Java --cpp
check "$foutput"

srcml sub/a.java --cpp -l Java
check "$foutput"

srcml sub/a.java --cpp -l Java -o sub/a.java.xml
check sub/a.java.xml "$foutput"

srcml sub/a.java -l Java --cpp -o sub/a.java.xml
check sub/a.java.xml "$foutput"

srcml sub/a.java -l Java -o sub/a.java.xml --cpp
check sub/a.java.xml "$foutput"

srcml sub/a.java -o sub/a.java.xml -l Java --cpp
check sub/a.java.xml "$foutput"

srcml sub/a.java -o sub/a.java.xml --cpp -l Java
check sub/a.java.xml "$foutput"

srcml sub/a.java --cpp -o sub/a.java.xml -l Java
check sub/a.java.xml "$foutput"

# from file - options before input
srcml -l Java --cpp sub/a.java
check "$foutput"

srcml --cpp -l Java sub/a.java
check "$foutput"

srcml --cpp -l Java -o sub/a.java.xml sub/a.java
check sub/a.java.xml "$foutput"

srcml -l Java --cpp -o sub/a.java.xml sub/a.java
check sub/a.java.xml "$foutput"

srcml -l Java -o sub/a.java.xml --cpp sub/a.java
check sub/a.java.xml "$foutput"

srcml -o sub/a.java.xml -l Java --cpp sub/a.java
check sub/a.java.xml "$foutput"

srcml -o sub/a.java.xml --cpp -l Java sub/a.java
check sub/a.java.xml "$foutput"

srcml --cpp -o sub/a.java.xml -l Java sub/a.java
check sub/a.java.xml "$foutput"

# from file - options intermixed with input
srcml --cpp -l Java sub/a.java -o sub/a.java.xml
check sub/a.java.xml "$foutput"

srcml -l Java --cpp sub/a.java -o sub/a.java.xml
check sub/a.java.xml "$foutput"

srcml -l Java -o sub/a.java.xml sub/a.java --cpp
check sub/a.java.xml "$foutput"

srcml -o sub/a.java.xml -l Java sub/a.java --cpp
check sub/a.java.xml "$foutput"

srcml -o sub/a.java.xml --cpp sub/a.java -l Java
check sub/a.java.xml "$foutput"

srcml --cpp -o sub/a.java.xml sub/a.java -l Java
check sub/a.java.xml "$foutput"

srcml --cpp sub/a.java -l Java -o sub/a.java.xml
check sub/a.java.xml "$foutput"

srcml -l Java sub/a.java --cpp -o sub/a.java.xml
check sub/a.java.xml "$foutput"

srcml -l Java sub/a.java -o sub/a.java.xml --cpp
check sub/a.java.xml "$foutput"

srcml -o sub/a.java.xml sub/a.java -l Java --cpp
check sub/a.java.xml "$foutput"

srcml -o sub/a.java.xml sub/a.java --cpp -l Java
check sub/a.java.xml "$foutput"

srcml --cpp sub/a.java -o sub/a.java.xml -l Java
check sub/a.java.xml "$foutput"

