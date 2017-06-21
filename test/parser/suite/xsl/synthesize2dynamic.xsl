<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:cpp="http://www.srcML.org/srcML/cpp"
	xmlns:str="http://exslt.org/strings"
	xmlns:func="http://exslt.org/functions"
	xmlns:exsl="http://exslt.org/common"
	extension-element-prefixes="str exsl func"
	exclude-result-prefixes="src cpp"
	version="1.0">

<!--
    synthesize2dynamic.xsl

    Convert @synthesize to @dynamic.

    Michael John Decker
    mdecker6@kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<xsl:template match="src:unit/@url">
    <xsl:attribute name="url"><xsl:value-of select="str:replace(., 'synthesize', 'dynamic')"/></xsl:attribute>
</xsl:template>

<xsl:template match="src:unit/@filename">
    <xsl:attribute name="filename"><xsl:value-of select="str:replace(., 'synthesize', 'dynamic')"/></xsl:attribute>
</xsl:template>

<xsl:template match="src:synthesize/text()">
    <xsl:value-of select="str:replace(., '@synthesize', '@dynamic')"/>
</xsl:template>

<xsl:template match="src:synthesize">
    <dynamic><xsl:apply-templates select="@*|node()"/></dynamic>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
