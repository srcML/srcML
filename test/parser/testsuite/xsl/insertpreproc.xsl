<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.srcML.org/srcML/src"
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:cpp="http://www.srcML.org/srcML/cpp"
	exclude-result-prefixes="src"
	version="1.0">

<!--
	insertpreproc.xsl

	Inserts preprocessor statements at every opportunity
-->

<xsl:import href="insertcomments.xsl"/>

<xsl:variable name="cmt" xml:space="preserve"><xsl:text>
</xsl:text><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define><xsl:text>
</xsl:text></xsl:variable> 

<!-- change url name -->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
		<xsl:value-of select="."/><xsl:text>.preproc</xsl:text>
	</xsl:attribute>
</xsl:template>

</xsl:stylesheet>
