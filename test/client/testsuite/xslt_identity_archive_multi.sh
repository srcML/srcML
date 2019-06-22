#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# xslt identity transformation (archive of single file)
define identity_xslt <<- 'STDOUT'
	<xsl:stylesheet
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:cpp="http://www.srcML.org/srcML/cpp"
	xmlns:src="http://www.srcML.org/srcML/src"
	version="1.0">
	<xsl:template match="@*|node()">
	  <xsl:copy>
	   <xsl:apply-templates select="@*|node()"/>
	  </xsl:copy>
	 </xsl:template>
	</xsl:stylesheet>
	STDOUT

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"
createfile identity.xsl "$identity_xslt"

# --xslt=identity.xsl
srcml --xslt=identity.xsl sub/a.cpp.xml
check "$srcml"

srcml --xslt=identity.xsl < sub/a.cpp.xml
check "$srcml"

srcml --xslt=identity.xsl sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt=identity.xsl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt=identity.xsl -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

# --xslt identity.xsl
srcml --xslt identity.xsl sub/a.cpp.xml
check "$srcml"

srcml --xslt identity.xsl < sub/a.cpp.xml
check "$srcml"

srcml --xslt identity.xsl sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt identity.xsl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt identity.xsl -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

# --xslt "identity.xsl"
srcml --xslt "identity.xsl" sub/a.cpp.xml
check "$srcml"

srcml --xslt "identity.xsl" < sub/a.cpp.xml
check "$srcml"

srcml --xslt "identity.xsl" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt "identity.xsl" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt "identity.xsl" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

# --xslt="identity.xsl"
srcml --xslt="identity.xsl" sub/a.cpp.xml
check "$srcml"

srcml --xslt="identity.xsl" < sub/a.cpp.xml
check "$srcml"

srcml --xslt="identity.xsl" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt="identity.xsl" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt="identity.xsl" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml "$srcml"
