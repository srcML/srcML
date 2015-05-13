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

createfile sub/a.java "$input"

# standard in
#src2srcml -l Java -cpp <<< "$input"
#check 3<<< "$output"
#
#src2srcml -cpp -l Java <<< "$input"
#check 3<<< "$output"
#
#src2srcml -l Java -cpp -o sub/a.java.xml <<< "$input"
#check sub/a.java.xml 3<<< "$output"
#
#src2srcml -l Java -o sub/a.java.xml -cpp <<< "$input"
#check sub/a.java.xml 3<<< "$output"
#
#src2srcml -o sub/a.java.xml -l Java -cpp <<< "$input"
#check sub/a.java.xml 3<<< "$output"
#
#src2srcml -o sub/a.java.xml -cpp -l Java <<< "$input"
#check sub/a.java.xml 3<<< "$output"
#
#src2srcml -cpp -o sub/a.java.xml -l Java <<< "$input"
#check sub/a.java.xml 3<<< "$output"
#
#src2srcml -cpp -l Java -o sub/a.java.xml <<< "$input"
#check sub/a.java.xml 3<<< "$output"

echo "$input" | src2srcml -l Java --cpp
check 3<<< "$output"

echo "$input" | src2srcml --cpp -l Java
check 3<<< "$output"

echo "$input" | src2srcml -l Java --cpp -o sub/a.java.xml
check sub/a.java.xml 3<<< "$output"

echo "$input" | src2srcml -l Java -o sub/a.java.xml --cpp
check sub/a.java.xml 3<<< "$output"

echo "$input" | src2srcml --cpp -l Java -o sub/a.java.xml
check sub/a.java.xml 3<<< "$output"

echo "$input" | src2srcml --cpp -o sub/a.java.xml -l Java
check sub/a.java.xml 3<<< "$output"

echo "$input" | src2srcml -o sub/a.java.xml -l Java --cpp
check sub/a.java.xml 3<<< "$output"

echo "$input" | src2srcml -o sub/a.java.xml --cpp -l Java
check sub/a.java.xml 3<<< "$output"

# from file - options after input
src2srcml sub/a.java -l Java --cpp
check 3<<< "$foutput"

src2srcml sub/a.java --cpp -l Java
check 3<<< "$foutput"

src2srcml sub/a.java --cpp -l Java -o sub/a.java.xml
check sub/a.java.xml 3<<< "$foutput"

src2srcml sub/a.java -l Java --cpp -o sub/a.java.xml
check sub/a.java.xml 3<<< "$foutput"

src2srcml sub/a.java -l Java -o sub/a.java.xml --cpp
check sub/a.java.xml 3<<< "$foutput"

src2srcml sub/a.java -o sub/a.java.xml -l Java --cpp
check sub/a.java.xml 3<<< "$foutput"

src2srcml sub/a.java -o sub/a.java.xml --cpp -l Java
check sub/a.java.xml 3<<< "$foutput"

src2srcml sub/a.java --cpp -o sub/a.java.xml -l Java
check sub/a.java.xml 3<<< "$foutput"

# from file - options before input
src2srcml -l Java --cpp sub/a.java
check 3<<< "$foutput"

src2srcml --cpp -l Java sub/a.java
check 3<<< "$foutput"

src2srcml --cpp -l Java -o sub/a.java.xml sub/a.java
check sub/a.java.xml 3<<< "$foutput"

src2srcml -l Java --cpp -o sub/a.java.xml sub/a.java
check sub/a.java.xml 3<<< "$foutput"

src2srcml -l Java -o sub/a.java.xml --cpp sub/a.java
check sub/a.java.xml 3<<< "$foutput"

src2srcml -o sub/a.java.xml -l Java --cpp sub/a.java
check sub/a.java.xml 3<<< "$foutput"

src2srcml -o sub/a.java.xml --cpp -l Java sub/a.java
check sub/a.java.xml 3<<< "$foutput"

src2srcml --cpp -o sub/a.java.xml -l Java sub/a.java
check sub/a.java.xml 3<<< "$foutput"

# from file - options intermixed with input
src2srcml --cpp -l Java sub/a.java -o sub/a.java.xml
check sub/a.java.xml 3<<< "$foutput"

src2srcml -l Java --cpp sub/a.java -o sub/a.java.xml
check sub/a.java.xml 3<<< "$foutput"

src2srcml -l Java -o sub/a.java.xml sub/a.java --cpp 
check sub/a.java.xml 3<<< "$foutput"

src2srcml -o sub/a.java.xml -l Java sub/a.java --cpp 
check sub/a.java.xml 3<<< "$foutput"

src2srcml -o sub/a.java.xml --cpp sub/a.java -l Java
check sub/a.java.xml 3<<< "$foutput"

src2srcml --cpp -o sub/a.java.xml sub/a.java -l Java
check sub/a.java.xml 3<<< "$foutput"

src2srcml --cpp sub/a.java -l Java -o sub/a.java.xml
check sub/a.java.xml 3<<< "$foutput"

src2srcml -l Java sub/a.java --cpp -o sub/a.java.xml
check sub/a.java.xml 3<<< "$foutput"

src2srcml -l Java sub/a.java -o sub/a.java.xml --cpp
check sub/a.java.xml 3<<< "$foutput"

src2srcml -o sub/a.java.xml sub/a.java -l Java --cpp 
check sub/a.java.xml 3<<< "$foutput"

src2srcml -o sub/a.java.xml sub/a.java --cpp -l Java
check sub/a.java.xml 3<<< "$foutput"

src2srcml --cpp sub/a.java -o sub/a.java.xml -l Java
check sub/a.java.xml 3<<< "$foutput"

