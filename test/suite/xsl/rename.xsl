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
	rename.xsl

	Rename a default variable NAME to a given passed in name.

	Michael John Decker
	mdecker6@kent.edu

	Adapted from Michael L. Collard's for2while.xsl
-->


<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8"/>

<!-- change the url attribute on any unit from "if" to "while" -->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
		<xsl:value-of select="concat(., '.', $new_name)"/>
	</xsl:attribute>
</xsl:template>

<!-- put in text identifier name -->
<xsl:template match="src:name/text()[1]">
	<xsl:value-of select="str:replace(., 'NAME', $new_name)"/>
</xsl:template>

<!-- put in text specifier name -->
<xsl:template match="src:specifier/text()[1]">
	<xsl:value-of select="str:replace(., 'NAME', $new_name)"/>
</xsl:template>

<!-- change the filename attribute on the unit from "if" to "while" -->
<xsl:template match="src:unit/@filename">
	<xsl:attribute name="filename">
		<xsl:value-of select="concat(., '.', $new_name)"/>
	</xsl:attribute>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
