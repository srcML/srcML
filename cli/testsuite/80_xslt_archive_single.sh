#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define identiy_xslt <<- 'STDOUT'
	<xsl:stylesheet
	xmlns="http://www.sdml.info/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	xmlns:src="http://www.sdml.info/srcML/src"
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
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	
	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$srcml"
createfile archive.xsl "$identiy_xslt"

# --xslt=archive.xsl
srcml2src --xslt=archive.xsl sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt=archive.xsl < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt=archive.xsl sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt=archive.xsl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt=archive.xsl -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

# --xslt archive.xsl
srcml2src --xslt archive.xsl sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt archive.xsl < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt archive.xsl sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt archive.xsl -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt archive.xsl -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

# --xslt "archive.xsl"
srcml2src --xslt "archive.xsl" sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt "archive.xsl" < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt "archive.xsl" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt "archive.xsl" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt "archive.xsl" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

# --xslt="archive.xsl"
srcml2src --xslt="archive.xsl" sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt="archive.xsl" < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt="archive.xsl" sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt="archive.xsl" -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"

srcml2src --xslt="archive.xsl" -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "xslt : archive.xsl"
