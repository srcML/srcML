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
<xsl:template match="src:decl[
		     substring(src:name, string-length(src:name) - 4)='_FLAG' or
		     substring(src:name, string-length(src:name) - 10)='_FLAG_SHORT' or
		     (contains(src:name, 'DEFAULT') and contains(src:name, 'ENCODING'))
]">

  <xsl:value-of select="concat('&lt;!ENTITY ', src:name, ' ' , src:init/src:expr, '&gt;')"/><xsl:text>
</xsl:text>

</xsl:template>

<!-- filter out text output -->
<xsl:template match="text()"/>

</xsl:stylesheet>
