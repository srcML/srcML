#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test register language

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="Java" filename=sub/a.cpp/>
	STDOUT
INPUT
f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml --register-ext 'cpp=Java' sub/a.cpp "" fsxmlfile)
src2srcml --register-ext + '=cpp=Java' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml --register-ext 'cpp=Java' sub/a.cpp -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --register-ext 1)
src2srcml -l C++ --register-ext "cpp=Jawa" )

define output <<- 'STDOUT'
	fsxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="Java" filename="sub/a.xml"/>
	STDOUT
INPUT
if platform.system() != "Windows":
	os.system("touch su
b/a.xml")
else :
	os.system("copy emptysrc\\empty.cpp sub\\a.xml")

src2srcml --register-ext 'xml=Java' 'sub/a.xml' "" fsxmlfile)
src2srcml --register-ext + '=xml=Java' 'sub/a.xml' "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml --register-ext 'xml=Java' 'sub/a.xml' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --register-ext "xml=Jawa" 1)
