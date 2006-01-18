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
	mergeunit.xsl

	Merges embedded units

	Michael L. Collard
	collard@mcs.kent.edu
-->

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<!-- filter out start and end elements  -->
<xsl:template match="/src:unit/src:unit">
	<xsl:apply-templates/>
</xsl:template>

<!-- change dir name -->
<xsl:template match="src:unit/@dir">
	<xsl:attribute name="dir">
		<xsl:value-of select="."/><xsl:text>.all</xsl:text>
	</xsl:attribute>
</xsl:template>

<!-- filter out comments -->
<xsl:template match="comment()"/>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
