<xsl:stylesheet
	xmlns="http://www.sdml.info/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	version="1.0">

<!--
	copy.xsl

	Identity transformation.

	Michael L. Collard
	collard@uakron.edu
-->

<xsl:output method="text"/>

<xsl:param name="name" select="'Error'"/>

<xsl:template match="/"><xsl:copy-of select="$name"/><xsl:text>
</xsl:text></xsl:template>

</xsl:stylesheet>
