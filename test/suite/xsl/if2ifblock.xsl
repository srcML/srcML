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

<!--
	if2blockif.xsl

	Converts srcML if statement test cases to if statement test cases with blocks around the then and else statements.

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<!-- change the filename attribute on the unit from "if" to "while" -->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">if</xsl:attribute>
</xsl:template>


<!-- filter the then elements, but not the contents of the then -->
<xsl:template match="src:if/src:then[not(.//src:if)]/src:* | src:if/src:else[not(.//src:if)]/src:*">
  <block>{ <xsl:choose><xsl:when test="@type='pseudo'"><xsl:copy-of select="node()"/></xsl:when><xsl:otherwise><xsl:copy-of select="."/></xsl:otherwise></xsl:choose> }</block>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
