<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:cpp="http://www.srcML.org/srcML/cpp"
	xmlns:str="http://exslt.org/strings"
	xmlns:func="http://exslt.org/functions"
	extension-element-prefixes="str func"
	exclude-result-prefixes="src"
	version="1.0">

<!--
	insertblock.xsl

	Inserts a block around the contents in each unit

	Michael L. Collard
	collard@uakron.edu
-->

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="ISO-8859-1" standalone="yes"/>

<!-- change the url attribute on any unit from "*" to "*.block"-->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
		<xsl:value-of select="."/><xsl:text>.block</xsl:text>
	</xsl:attribute>
</xsl:template>

<!-- wrap the entire contents of the unit inside a block -->
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
