#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test xslt on single unit
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define identity <<- 'STDOUT'
	<?xml version="1.0"?>
	<xsl:stylesheet
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:src="http://www.srcML.org/srcML/src"
		xmlns="http://www.srcML.org/srcML/src"
		xmlns:cpp="http://www.srcML.org/srcML/cpp"
		xmlns:str="http://exslt.org/strings"
		xmlns:func="http://exslt.org/functions"
		xmlns:exsl="http://exslt.org/common"
		extension-element-prefixes="str exsl func"
		exclude-result-prefixes="src"
		version="1.0">
	  <xsl:template match="@* | node()">
	      <xsl:copy>
	         <xsl:apply-templates select="@* | node()"/>
	      </xsl:copy>
	   </xsl:template>
	</xsl:stylesheet>
	STDOUT

xmlcheck "$srcml"
createfile sub/unit.cpp.xml "$srcml"
createfile identity.xsl "$identity"

# simple copy
srcml sub/unit.cpp.xml --xslt=identity.xsl
check "$srcml"

srcml --xslt=identity.xsl sub/unit.cpp.xml
check "$srcml"

srcml --xslt=identity.xsl < sub/unit.cpp.xml
check "$srcml"

srcml sub/unit.cpp.xml --xslt=identity.xsl -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml sub/unit.cpp.xml -o sub/b.cpp.xml --xslt=identity.xsl
check sub/b.cpp.xml "$srcml"

srcml --xslt=identity.xsl sub/unit.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt=identity.xsl -o sub/b.cpp.xml sub/unit.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt=identity.xsl -o sub/b.cpp.xml < sub/unit.cpp.xml
check sub/b.cpp.xml "$srcml"

# xslt apply root copy
srcml sub/unit.cpp.xml --xslt=identity.xsl
check "$srcml"

srcml sub/unit.cpp.xml --xslt=identity.xsl
check "$srcml"

srcml --xslt=identity.xsl sub/unit.cpp.xml
check "$srcml"

srcml --xslt=identity.xsl < sub/unit.cpp.xml
check "$srcml"

srcml --xslt=identity.xsl sub/unit.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt=identity.xsl -o sub/b.cpp.xml sub/unit.cpp.xml
check sub/b.cpp.xml "$srcml"

srcml --xslt=identity.xsl -o sub/b.cpp.xml < sub/unit.cpp.xml
check sub/b.cpp.xml "$srcml"

