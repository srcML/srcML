#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file transformation_order.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
#
# Transformations are applied in the order they occur on the command line,
# whether given as an option, i.e., --xpath, --srcql, --xslt, --relaxng,
# or as a transformation filename, i.e., a .xsl or .rng file

# test framework
source $(dirname "$0")/framework_test.sh

defineXML srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

# removes every expr_stmt, so a following --xpath for a name finds nothing
defineXML delexpr <<- 'STDOUT'
	<xsl:stylesheet
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	exclude-result-prefixes="src"
	version="1.0">
	<xsl:template match="@*|node()">
	  <xsl:copy>
	   <xsl:apply-templates select="@*|node()"/>
	  </xsl:copy>
	 </xsl:template>
	 <xsl:template match="src:expr_stmt"/>
	</xsl:stylesheet>
STDOUT

# renames every name to a literal
defineXML rename <<- 'STDOUT'
	<xsl:stylesheet
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	exclude-result-prefixes="src"
	version="1.0">
	<xsl:template match="@*|node()">
	  <xsl:copy>
	   <xsl:apply-templates select="@*|node()"/>
	  </xsl:copy>
	 </xsl:template>
	 <xsl:template match="src:name">
	  <literal><xsl:apply-templates select="@*|node()"/></literal>
	 </xsl:template>
	</xsl:stylesheet>
STDOUT

# removes every literal, so it only has an effect after the rename
defineXML delliteral <<- 'STDOUT'
	<xsl:stylesheet
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	exclude-result-prefixes="src"
	version="1.0">
	<xsl:template match="@*|node()">
	  <xsl:copy>
	   <xsl:apply-templates select="@*|node()"/>
	  </xsl:copy>
	 </xsl:template>
	 <xsl:template match="src:literal"/>
	</xsl:stylesheet>
STDOUT

# replaces every name with the value of the tag parameter
defineXML param <<- 'STDOUT'
	<xsl:stylesheet
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	exclude-result-prefixes="src"
	version="1.0">
	<xsl:param name="tag">none</xsl:param>
	<xsl:template match="@*|node()">
	  <xsl:copy>
	   <xsl:apply-templates select="@*|node()"/>
	  </xsl:copy>
	 </xsl:template>
	 <xsl:template match="src:name">
	  <comment type="line"><xsl:value-of select="$tag"/></comment>
	 </xsl:template>
	</xsl:stylesheet>
STDOUT

# only a name is a valid document, so it only matches the result of an --xpath for a name
defineXML nameschema <<- 'STDOUT'
	<grammar xmlns="http://relaxng.org/ns/structure/1.0" ns="http://www.srcML.org/srcML/src">
	  <start>
	    <element name="name">
	      <text/>
	    </element>
	  </start>
	</grammar>
STDOUT

createfile a.cpp.xml "$srcml"
createfile delexpr.xsl "$delexpr"
createfile rename.xsl "$rename"
createfile delliteral.xsl "$delliteral"
createfile param.xsl "$param"
createfile name.rng "$nameschema"

# empty, as the xslt removed the expr_stmt before the xpath was applied
defineXML empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
STDOUT

# the name found by the xpath, as the xslt had no expr_stmt left to remove
defineXML aname <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp">a</unit>

	</unit>
STDOUT

# xslt before xpath
srcml a.cpp.xml --xslt delexpr.xsl --xpath="//src:name"
check "$empty"

srcml a.cpp.xml delexpr.xsl --xpath="//src:name"
check "$empty"

# xpath before xslt
srcml a.cpp.xml --xpath="//src:name" --xslt delexpr.xsl
check "$aname"

srcml a.cpp.xml --xpath="//src:name" delexpr.xsl
check "$aname"

# relaxng before xpath, where the schema does not match the whole unit
# the schema failure is reported by libxml2, so it is not checked
srcml a.cpp.xml --relaxng name.rng --xpath="//src:name" 2>/dev/null
check "$empty"

srcml a.cpp.xml name.rng --xpath="//src:name" 2>/dev/null
check "$empty"

# xpath before relaxng, where the schema matches the name found by the xpath
srcml a.cpp.xml --xpath="//src:name" --relaxng name.rng
check "$aname"

srcml a.cpp.xml --xpath="//src:name" name.rng
check "$aname"

# the literal produced by the rename is removed
defineXML noexpr <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr/>;</expr_stmt>
	</unit>
STDOUT

# nothing to remove yet, so the rename is what shows
defineXML literal <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><literal>a</literal></expr>;</expr_stmt>
	</unit>
STDOUT

# two xslt programs, in both orders
srcml a.cpp.xml --xslt rename.xsl --xslt delliteral.xsl
check "$noexpr"

srcml a.cpp.xml rename.xsl delliteral.xsl
check "$noexpr"

srcml a.cpp.xml --xslt delliteral.xsl --xslt rename.xsl
check "$literal"

srcml a.cpp.xml delliteral.xsl rename.xsl
check "$literal"

# the xslt parameter applies to its xslt program, whether it follows or precedes it
defineXML tagged <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><comment type="line">first</comment></expr>;</expr_stmt>
	</unit>
STDOUT

srcml a.cpp.xml --xslt param.xsl --xslt-param tag='"first"'
check "$tagged"

srcml a.cpp.xml --xslt-param tag='"first"' param.xsl
check "$tagged"

srcml a.cpp.xml --xslt-param tag='"first"' --xslt param.xsl
check "$tagged"
