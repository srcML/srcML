#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test xslt empty input
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define copyxslt <<- 'STDOUT'
	<xsl:stylesheet
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:cpp="http://www.srcML.org/srcML/cpp"
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

createfile sub/a.cpp.xml "$srcml"
createfile copy.xsl "$copyxslt"


# xslt copy xpathparam NAME=VALUE
srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' sub/a.cpp.xml
check 3<<< "$srcml"

srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' < sub/a.cpp.xml
check 3<<< "$srcml"

srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml --apply-root --xslt=copy.xsl --xpathparam 'NAME=VALUE' -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"


# xslt copy xpathparam name="a"
srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' sub/a.cpp.xml
check 3<<< "$srcml"

srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' < sub/a.cpp.xml
check 3<<< "$srcml"

srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

srcml --apply-root --xslt=copy.xsl --xpathparam 'name="a"' -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml"

