<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
<!--
    @file insertpreproc.xsl

    @copyright Copyright (C) 2019-2024 srcML, LLC. (www.srcML.org)

    Inserts preprocessor statements at every opportunity
-->

<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    exclude-result-prefixes="src"
    version="1.0">

<xsl:import href="copy.xsl"/>

<xsl:variable name="cmt" xml:space="preserve"><xsl:text>
</xsl:text><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define><xsl:text>
</xsl:text></xsl:variable> 

<xsl:template match="src:unit">
    <unit xmlns:cpp="http://www.srcML.org/srcML/cpp">
        <xsl:apply-templates select="@*|node()"/>           
    </unit>
</xsl:template>

<xsl:template match="src:name | src:comment | src:literal | cpp:*">
  <xsl:copy-of select="."/>
</xsl:template>

<xsl:template match="src:unit">
    <unit xmlns:cpp="http://www.srcML.org/srcML/cpp">
        <xsl:apply-templates select="@*|node()"/>
    </unit>
</xsl:template>

<xsl:template match="text()[contains(., ' ')]">
  <xsl:value-of select="substring-before(., ' ')"/>
  <xsl:copy-of select="$cmt"/>
  <xsl:value-of select="substring-after(., ' ')"/>
</xsl:template>

</xsl:stylesheet>
