<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    xmlns:str="http://exslt.org/strings"
    extension-element-prefixes="str"
    exclude-result-prefixes="src"
    version="1.0">

<!--
    keyword2mkeyword.xsl

    Converts try/catch/finally/throw keywords to @try/@catch/@finally/@throw keywords
-->

<xsl:import href="copy.xsl"/>

<xsl:template match="src:try/text()[1]">
    <xsl:value-of select="str:replace(., 'try', '@try')"/>
</xsl:template>

<xsl:template match="src:catch/text()[1]">
    <xsl:value-of select="str:replace(., 'catch', '@catch')"/>
</xsl:template>

<xsl:template match="src:finally/text()[1]">
    <xsl:value-of select="str:replace(., 'finally', '@finally')"/>
</xsl:template>

<xsl:template match="src:throw/text()[1]">
    <xsl:value-of select="str:replace(., 'throw', '@throw')"/>
</xsl:template>

<!-- trap the current language attribute -->
<xsl:template match="src:unit/@language">
    <xsl:attribute name="language">Objective-C</xsl:attribute>
</xsl:template>

</xsl:stylesheet>
