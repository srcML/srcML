<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.sdml.info/srcML/src"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	version="1.0">

<!--
	extract_return_status.xsl

	Extracts return status from the srcML source files.

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:output method="text"/>

<!-- output an entity for a declaration -->
<xsl:template match="src:enum[src:name='RETURN_STATUS']/src:block/src:expr">
  <xsl:value-of select="concat('&lt;!ENTITY ', src:name, ' &quot;' , src:literal, '&quot;&gt;')"/><xsl:text>
</xsl:text>
</xsl:template>

<!-- filter out text output -->
<xsl:template match="text()"/>

</xsl:stylesheet>
