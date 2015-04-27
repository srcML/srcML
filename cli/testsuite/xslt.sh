#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test xslt empty input
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define copyxslt <<- 'STDOUT'
	<xsl:stylesheet
	xmlns="http://www.sdml.info/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	version="1.0">

	<!--
	copy.xsl

	Identity transformation.

	Michael L. Collard
	collard@uakron.edu
	-->

	<xsl:output method="xml" version="1.0" encoding="UTF-8" standalone="yes"/>

	<xsl:template match="@*|node()"><xsl:copy><xsl:apply-templates select="@*|node()"/></xsl:copy></xsl:template>

	</xsl:stylesheet>
	STDOUT

define xpath_err1 <<- 'STDERR'
	xslt : copy.xsl
	STDERR

define xpath_err2 <<- 'STDERR'
	xslt : copy.xsl
	xpathparam : NAME=VALUE
	STDERR

define xpath_err3 <<- 'STDERR'
	xslt : copy.xsl
	xpathparam : name="a"
	STDERR

createfile sub/a.cpp.xml "$srcml"
createfile copy.xsl "$copyxslt"

# simple copy
srcml --xslt=copy.xsl sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "$xpath_err1"

srcml --xslt=copy.xsl < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "$xpath_err1"

srcml --xslt=copy.xsl sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err1"

srcml --xslt=copy.xsl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err1"

srcml --xslt=copy.xsl -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err1"

# xslt apply root copy
srcml --apply-root --xslt=copy.xsl sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "$xpath_err1"

srcml --apply-root --xslt=copy.xsl < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "$xpath_err1"

srcml --apply-root --xslt=copy.xsl sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err1"

srcml --apply-root --xslt=copy.xsl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err1"

srcml --apply-root --xslt=copy.xsl -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err1"


# xslt copy xpathparam NAME=VALUE
srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "$xpath_err2"

srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "$xpath_err2"

srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err2"

srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err2"

srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err2"


# xslt copy xpathparam name="a"
srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "$xpath_err3"

srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "$xpath_err3"

srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err3"

srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err3"

srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$xpath_err3"

