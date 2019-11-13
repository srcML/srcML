<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    exclude-result-prefixes="src"
    version="1.0">

<!--
    nestif.xsl

    Contents are duplicated to then and else parts of if
-->

<xsl:import href="copy.xsl"/>

<!-- wrap the entire contents of the unit inside a block -->
<xsl:template match="src:unit/src:unit"><xsl:copy><xsl:copy-of select="@*"/><xsl:text>
</xsl:text><if_stmt><if>if <condition>(<expr><name>cond</name></expr>)</condition> <block>{<block_content>
    <xsl:copy-of select="*|text()"/></block_content>}</block></if>
<else>else <block>{<block_content>
    <xsl:copy-of select="*|text()"/></block_content>}</block></else></if_stmt><xsl:text>
</xsl:text></xsl:copy>
</xsl:template>

</xsl:stylesheet>
