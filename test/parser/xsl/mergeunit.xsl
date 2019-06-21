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
    mergeunit.xsl

    Merges embedded units
-->

<xsl:import href="copy.xsl"/>

<!-- create a new root element -->
<xsl:template match="/src:unit">

     <xsl:choose>
        <xsl:when test="/src:unit/src:unit/@language='Java'">
    <unit xmlns="http://www.srcML.org/srcML/src">
<!--    <xsl:attribute name="url">
        <xsl:value-of select="@url"/><xsl:text>.all</xsl:text>
    </xsl:attribute>
-->
        <xsl:copy-of select="/src:unit/src:unit/@*"/>
    <xsl:apply-templates/>
    </unit>
        </xsl:when>
        <xsl:otherwise>
    <unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp">
<!--
    <xsl:attribute name="url">
        <xsl:value-of select="@url"/><xsl:text>.all</xsl:text>
    </xsl:attribute>
-->
        <xsl:copy-of select="/src:unit/src:unit/@*"/>
    <xsl:apply-templates/>
    </unit>
        </xsl:otherwise>
    </xsl:choose>

</xsl:template>

<!-- filter out start and end elements  -->
<xsl:template match="/src:unit/src:unit">
    <xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>
