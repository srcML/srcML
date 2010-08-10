<xsl:stylesheet
	xmlns="http://www.sdml.info/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	xmlns:src="http://www.sdml.info/srcML/src"
	version="1.0">

<!--
	copy.xsl

	Identity transformation.

	Michael L. Collard
	collard@uakron.edu
-->

<xsl:output method="text"/>

<xsl:template match="/"><xsl:copy-of select="src:archive('filename')"/><xsl:text>
</xsl:text></xsl:template>

</xsl:stylesheet>
