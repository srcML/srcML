#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test directory input
define output <<- 'STDOUT'
	srcmlstart = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	STDOUT
INPUT
aj = STDOUT
<unit language="AspectJ" filename="dir/file.aj">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

c = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="dir/file.c">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

cpp = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="dir/file.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

java = STDOUT
<unit language="Java" filename="dir/file.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT



srcmlend = STDOUT
</unit>
	STDOUT

if platform.system() != "Windows" :
	dir = execute(['ls' 'dir'.split("\n")
else :
	dir = os.listdir('dir')

srcml = srcmlstart

for file in dir :
	if file == 'file.aj' :
	srcml += aj
	if file == 'file.c' :
	srcml += c
	if file == 'file.cpp' :
	srcml += cpp
	if file == 'file.java' :
	srcml += java

srcml += srcmlend

if platform.system() == "Windows" or sys.platform == 'cygwin' :
	srcml = string.replace(srcml "dir/" "dir\\")

echo -n "" | src2srcml dir
src2srcml dir' -o 'dir/dir.xml' ""
readfile input dir/dir.xml' 'r').read() srcml)

if platform.system() != "Windows" and sys.platform != 'cygwin' :
	execute(['tar' 'czf' 'dir/foo.tar' 'dir/file.c'

echo -n "" |	 src2srcml dir
	src2srcml dir' -o 'dir/dir.xml' ""
	readfile input dir/dir.xml' 'r').read() srcml)

	execute(['rm' 'dir/foo.tar'

#
# nested files

src = STDOUT
a;
	STDOUT

