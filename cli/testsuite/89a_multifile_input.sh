#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# test
define output <<- 'STDOUT'
	srcmlstart = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	STDOUT
INPUT
cpp = STOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

cppempty= STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

java = STDOUT
<unit language="Java" filename="sub/a.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOU

javaempty = STDOUT
<unit language="Java" filename="sub/a.java"/>
	STDOUT



srcmlend = STDOT
</unit>
	STDOUT

if platform.system() != "Windows" :
	os.system('rm sub/a.cpp; touch sub/a.cpp)
else :
	os.system("del sub\\a.cpp")
	os.system("copy emptysrc\\empty.cpp sub\\a.cpp")

f = open('sub/a.java' 'w')
f.write(src)
f.close()

src2srcml sub/a.cpp 'sub/a.java' '' srcmlstart + cppempty + java + srcmlend)
src2srcml sub/a.cpp 'sub/a.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cppempty + java + srcmlend)

src2srcml sub/a.java' sub/a.cpp '' srcmlstart + java + cppempty + srcmlend)
src2srcml sub/a.java' sub/a.cpp -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + java + cppempty + srcmlend)

if platform.system() != "Windows" :
	os.system('rm sub/a.java; touch sub/a.java')
else :
	os.system("del sub\\a.java")
	os.system("copy emptysrc\\empty.java sub\\a.java")

f = open(sub/a.cpp 'w')
f.write(src)
f.close()

src2srcml sub/a.cpp 'sub/a.java' '' srcmlstart + cpp + javaempty + srcmlend)
src2srcml sub/a.cpp 'sub/a.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cpp + javaempty + srcmlend)

src2srcml sub/a.java' sub/a.cpp '' srcmlstart + javaempty + cpp + srcmlend)
src2srcml sub/a.java' sub/a.cpp -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + javaempty + cpp + srcmlend)

cpp = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/b.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

if platform.system() != "Windows" :
	os.system('rm sub/a.cpp; touch sub/a.cpp)
else :
	os.system("del sub\\a.cpp")
	os.system("copy emptysrc\\empty.cpp sub\\a.cpp")

f = open('sub/b.cpp' 'w')
f.write(src)
f.close()

src2srcml sub/a.cpp 'sub/b.cpp' '' srcmlstart + cppempty + cpp + srcmlend)
src2srcml sub/a.cpp 'sub/b.cpp' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cppempty + cpp + srcmlend)

src2srcml sub/b.cpp' sub/a.cpp '' srcmlstart + cpp + cppempty + srcmlend)
src2srcml sub/b.cpp' sub/a.cpp -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cpp + cppempty + srcmlend)

java = STDOUT
<unit language="Java" filename="sub/b.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

if platform.system() != "Windows" :
	os.system('rm sub/a.java; touch sub/a.java')
else :
	os.system("del sub\\a.java")
	os.system("copy emptysrc\\empty.java sub\\a.java")

f = open('sub/b.java' 'w')
f.write(src)
f.close()

src2srcml sub/a.java' 'sub/b.java' '' srcmlstart + javaempty + java + srcmlend)
src2srcml sub/a.java' 'sub/b.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + javaempty + java + srcmlend)

src2srcml sub/b.java' 'sub/a.java' '' srcmlstart + java + javaempty + srcmlend)
src2srcml sub/b.java' 'sub/a.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + java + javaempty + srcmlend)
