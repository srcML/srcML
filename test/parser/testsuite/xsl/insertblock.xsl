<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	xmlns="http://www.srcML.org/srcML/src"
	exclude-result-prefixes="src"
	version="1.0">

<!--
	insertblock.xsl

	Inserts a block around the contents in each unit
-->

<xsl:import href="copy.xsl"/>

<!-- wrap the entire contents of the unit inside a block -->
<xsl:template match="src:unit">
<xsl:copy>
<block>{
<xsl:copy-of select="*|text()"/>
}</block>
</xsl:copy>
</xsl:template>

</xsl:stylesheet>
