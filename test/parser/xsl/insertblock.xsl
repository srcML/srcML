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

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<!-- wrap the entire contents of the unit inside a block -->
<xsl:template match="src:unit">
<xsl:copy>
<block>{<block_content><xsl:text>
</xsl:text><xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text></block_content>}</block>
</xsl:copy>
</xsl:template>

</xsl:stylesheet>
