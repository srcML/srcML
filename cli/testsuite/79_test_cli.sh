#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT
INPUT
xslt_error = STDOUTsrcml2src: Start tag expected '<' not found in '-'
	STDOUT

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


# xslt

if sys.platform != 'cygwin' :
	checkError([srcml2src --xslt + '=copy.xsl' "" xslt_error)
	srcml2src --xslt + '=copy.xsl

	check 4<<< "2"

srcml2src --xslt + '=copy.xsl' srcml srcml)
echo -n "" | srcml2src --xslt + '=copy.xsl' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=copy.xsl' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xslt + '=copy.xsl' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xslt srcml

check 4<<< "1"
srcml2src --xslt + '=' srcml

check 4<<< "1"

# xslt apply root

if sys.platform != 'cygwin' :
	checkError([srcml2src --apply-root --xslt + '=copy.xsl' "" xslt_error)
	srcml2src --apply-root --xslt + '=copy.xsl

	check 4<<< "2"

srcml2src --apply-root --xslt + '=copy.xsl' srcml srcml)
echo -n "" | srcml2src --apply-root --xslt + '=copy.xsl' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --apply-root --xslt + '=copy.xsl' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --apply-root --xslt + '=copy.xsl' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --apply-root --xslt srcml

check 4<<< "1"
srcml2src --apply-root --xslt + '=' srcml

check 4<<< "1"

# param

srcml2src --xslt + '=copy.xsl' --xpathparam 'NAME=VALUE' srcml srcml)
echo -n "" | srcml2src --xslt + '=copy.xsl' --xpathparam 'NAME=VALUE' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=copy.xsl' --xpathparam 'NAME=VALUE' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xslt + '=copy.xsl' --xpathparam 'NAME=VALUE' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xslt + '=copy.xsl' --xpathparam srcml

check 4<<< "1"
srcml2src --xslt + '=copy.xsl' --xpathparam "NAME" srcml

check 4<<< "1"

xslt = STDOUTa
	STDOUT

srcml2src --xslt + '=param.xsl' --xpathparam 'name="a"' srcml xslt)
srcml2src --xslt + '=param.xsl' --xpathparam 'name="a"' sub/a.cpp.xml xslt)
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=param.xsl' --xpathparam 'name="a"' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xslt)
srcml2src --xslt + '=param.xsl' --xpathparam 'name="a"' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xslt)


srcml2src --xslt + '=copy.xsl' --xpathparam srcml

check 4<<< "1"
srcml2src --xslt + '=copy.xsl' --xpathparam "name" srcml

check 4<<< "1"

# stringparam

srcml2src --xslt + '=copy.xsl' --param 'NAME=VALUE' srcml srcml)
echo -n "" | srcml2src --xslt + '=copy.xsl' --param 'NAME=VALUE' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=copy.xsl' --param 'NAME=VALUE' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xslt + '=copy.xsl' --param 'NAME=VALUE' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xslt + '=copy.xsl' --param srcml

check 4<<< "1"
srcml2src --xslt + '=copy.xsl' --param "NAME" srcml

check 4<<< "1"

xslt = STDOUTa
	STDOUT

srcml2src --xslt + '=param.xsl' --param 'name=a' srcml xslt)
srcml2src --xslt + '=param.xsl' --param 'name=a' sub/a.cpp.xml xslt)
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=param.xsl' --param 'name=a' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xslt)
srcml2src --xslt + '=param.xsl' --param 'name=a' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xslt)


srcml2src --xslt + '=copy.xsl' --param srcml

check 4<<< "1"
srcml2src --xslt + '=copy.xsl' --param "name" srcml

check 4<<< "1"

