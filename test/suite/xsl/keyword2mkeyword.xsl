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
    keyword2mkeyword.xsl

    Converts try/catch/finally/throw keywords to @try/@catch/@finally/@throw keywords

    Modified from: setlanguage.xsl
    Originally by:
	Michael L. Collard
	collard@cs.kent.edu

    Michael John Decker
    mdecker6@kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<xsl:template match="src:try/text()">
    <xsl:value-of select="str:replace(., 'try', '@try')"/>
</xsl:template>

<xsl:template match="src:catch/text()">
    <xsl:value-of select="str:replace(., 'catch', '@catch')"/>
</xsl:template>

<xsl:template match="src:finally/text()">
    <xsl:value-of select="str:replace(., 'finally', '@finally')"/>
</xsl:template>

<xsl:template match="src:throw/text()">
    <xsl:value-of select="str:replace(., 'throw', '@throw')"/>
</xsl:template>

<!-- trap the current language attribute -->
<xsl:template match="/src:unit/@language">
    <xsl:attribute name="language">Objective-C</xsl:attribute>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
