<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    xmlns:str="http://exslt.org/strings"
    xmlns:func="http://exslt.org/functions"
    xmlns:exsl="http://exslt.org/common"
    extension-element-prefixes="str exsl func"
    exclude-result-prefixes="src"
    version="1.0">

<!--
    if2blockif.xsl

    Converts srcML if statement test cases to if statement test cases with blocks around the then and else statements.
-->

<xsl:import href="copy.xsl"/>

<!-- filter the then elements, but not the contents of the then -->
<xsl:template match="src:if_stmt/src:if[not(.//src:if)]/src:block[@type='pseudo'] | src:if_stmt/src:else[not(.//src:if_stmt)]/src:*">
  <block>{ <xsl:choose><xsl:when test="@type='pseudo'"><xsl:copy-of select="node()"/></xsl:when><xsl:otherwise><xsl:copy-of select="."/></xsl:otherwise></xsl:choose> }</block>
</xsl:template>

</xsl:stylesheet>
