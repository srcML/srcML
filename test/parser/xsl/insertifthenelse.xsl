<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:omp="http://www.srcML.org/srcML/openmp"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    exclude-result-prefixes="src"
    version="1.0">

<!--
    nestif.xsl

    Contents are duplicated to then and else parts of if
-->

<xsl:import href="copy.xsl"/>

<xsl:template match="src:unit[.//cpp:* and not(.//omp:*)]">
    <unit xmlns:cpp="http://www.srcML.org/srcML/cpp">
        <xsl:apply-templates select="@*|node()"/><xsl:text>
</xsl:text><if_stmt><if>if <condition>(<expr><name>cond</name></expr>)</condition> <block>{<block_content><xsl:text>
</xsl:text>    <xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text></block_content>}</block></if><xsl:text>
</xsl:text><else>else <block>{<block_content><xsl:text>
</xsl:text>    <xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text></block_content>}</block></else></if_stmt><xsl:text>
</xsl:text>           
    </unit>
</xsl:template>

<xsl:template match="src:unit[.//cpp:* and (.//omp:*)]">
    <unit xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:omp="http://www.srcML.org/srcML/openmp">
        <xsl:apply-templates select="@*|node()"/><xsl:text>
</xsl:text><if_stmt><if>if <condition>(<expr><name>cond</name></expr>)</condition> <block>{<block_content><xsl:text>
</xsl:text>    <xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text></block_content>}</block></if><xsl:text>
</xsl:text><else>else <block>{<block_content><xsl:text>
</xsl:text>    <xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text></block_content>}</block></else></if_stmt><xsl:text>
</xsl:text>
    </unit>
</xsl:template>

<!-- wrap the entire contents of the unit inside a block -->
<xsl:template match="src:unit[not(.//cpp:*) and not(.//omp:*)]"><xsl:copy><xsl:copy-of select="@*"/><xsl:text>
</xsl:text><if_stmt><if>if <condition>(<expr><name>cond</name></expr>)</condition> <block>{<block_content><xsl:text>
</xsl:text>    <xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text></block_content>}</block></if><xsl:text>
</xsl:text><else>else <block>{<block_content><xsl:text>
</xsl:text>    <xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text></block_content>}</block></else></if_stmt><xsl:text>
</xsl:text></xsl:copy>
</xsl:template>

</xsl:stylesheet>
