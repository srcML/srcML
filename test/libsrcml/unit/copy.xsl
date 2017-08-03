<xsl:stylesheet
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:cpp="http://www.srcML.org/srcML/cpp"
	version="1.0">

<!--
	copy.xsl

	Identity transformation.

	Michael L. Collard
	collard@uakron.edu
-->

<xsl:output method="xml" version="1.0" encoding="UTF-8" standalone="yes"/>

<xsl:template match="@*|node()"><xsl:copy><xsl:apply-templates select="@*|node()"/></xsl:copy></xsl:template>

</xsl:stylesheet>
