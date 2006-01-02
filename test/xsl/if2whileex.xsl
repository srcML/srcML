<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.sdml.info/srcML/src"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	xmlns:str="http://exslt.org/strings"
	xmlns:func="http://exslt.org/functions"
	xmlns:exsl="http://exslt.org/common"
	extension-element-prefixes="str exsl func"
	exclude-result-prefixes="src"
	version="1.0">

<!--
	if2while.xsl

	Converts srcML if statement test cases to while statement test cases

	Michael L. Collard
	collard@mcs.kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="ISO-8859-1"/>

<!-- change the directory attribute on any unit from "if" to "while" -->
<xsl:template match="src:unit/@dir">
	<xsl:attribute name="dir">
		<xsl:value-of select="str:replace(., 'if', 'while')"/>
	</xsl:attribute>
</xsl:template>

<!-- filter out any if statements with an else -->
<xsl:template match="src:unit[src:if//src:else]"/>

<!-- filter the then elements, but not the contents of the then -->
<xsl:template match="src:if/src:then">
	<xsl:apply-templates/>
</xsl:template>

<!-- change the if element to a while element -->
<xsl:template match="src:if[not(.//src:else)]">
	<src:while>
		<xsl:apply-templates select="*|@*|text()"/>
	</src:while>
</xsl:template>

<!-- change the if keyword to a while keyword -->
<xsl:template match="src:if/text()[1]">
	<xsl:value-of select="str:replace(., 'if', 'while')"/>
</xsl:template>

<!-- change the filename attribute on the unit from "if" to "while" -->
<xsl:template match="src:unit/@filename">
	<xsl:attribute name="filename">
		<xsl:value-of select="str:replace(., 'if', 'while')"/>
	</xsl:attribute>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
