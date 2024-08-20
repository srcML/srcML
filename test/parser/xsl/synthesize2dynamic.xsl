<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
<!--
    @file synthesize2dynamic.xsl

    @copyright Copyright (C) 2019-2024 srcML, LLC. (www.srcML.org)

    Convert @synthesize to @dynamic.
-->

<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    xmlns:str="http://exslt.org/strings"
    extension-element-prefixes="str"
    exclude-result-prefixes="src cpp"
    version="1.0">

<xsl:import href="copy.xsl"/>

<!-- convert synthesize element to dynamic element -->
<xsl:template match="src:synthesize">
    <dynamic><xsl:apply-templates select="@*|node()"/></dynamic>
</xsl:template>

<!-- convert synthesize keyword to dynamic-->
<xsl:template match="src:synthesize/text()[1]">
    <xsl:value-of select="str:replace(., '@synthesize', '@dynamic')"/>
</xsl:template>

</xsl:stylesheet>
