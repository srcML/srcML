<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:str="http://exslt.org/strings"
	xmlns:func="http://exslt.org/functions"
	xmlns:exsl="http://exslt.org/common"
	extension-element-prefixes="str exsl func"
	exclude-result-prefixes="src"
	version="1.0">

<!--
	insertblock.java.xsl

	Inserts a block around each unit for Java

	Michael L. Collard
	collard@uakron.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="ISO-8859-1" standalone="yes"/>

<!-- change the url attribute on any unit from "*" to "*.block"-->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
		<xsl:value-of select="."/><xsl:text>.block</xsl:text>
	</xsl:attribute>
</xsl:template>

<!-- filter out any if statements with an else -->
<xsl:template match="src:unit/src:unit" xml:space="preserve">
<unit>
<block>{
<xsl:copy-of select="*|text()"/>
}</block>
</unit>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
