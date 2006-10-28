<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.sdml.info/srcML/src"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	version="1.0">

<!--
	extract_option.xsl

	Extracts flag options from the srcML source files.

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:output method="text"/>

<!-- output an entity for a declaration -->
<xsl:template match="src:decl[contains(src:name, '_FLAG')]">

  <xsl:value-of select="concat(src:name, '=', src:init/src:expr)"/><xsl:text>
</xsl:text>

</xsl:template>

<!-- filter out text output -->
<xsl:template match="text()"/>

</xsl:stylesheet>
