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
	insertstruct.xsl

	Inserts a struct around the contents in each unit

	Michael L. Collard
	collard@uakron.edu
-->

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<!-- change the url attribute on any unit from "*" to "*.block"-->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
		<xsl:value-of select="."/><xsl:text>.struct</xsl:text>
	</xsl:attribute>
</xsl:template>

<!-- wrap the entire contents of the unit inside a block -->
<xsl:template match="src:unit/src:unit" xml:space="preserve">
<unit>
<struct>struct <name>S</name> <block>{<public type="default"><xsl:copy-of select="*|text()"/></public>}</block>;</struct>
</unit>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
