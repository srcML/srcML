<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
<!--
    @file interface2implementation.xsl

    @copyright Copyright (C) 2019-2024 srcML, LLC. (www.srcML.org)

    Converts srcML Objective-C class interface to class implementation
-->

<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    xmlns:str="http://exslt.org/strings"
    xmlns:func="http://exslt.org/functions"
    xmlns:exsl="http://exslt.org/common"
    extension-element-prefixes="str exsl func"
    exclude-result-prefixes="src cpp"
    version="1.0">

<xsl:import href="copy.xsl"/>

<!-- change the class keyword to a interface keyword -->
<xsl:template match="src:class/@type">
    <xsl:attribute name="type">@implementation</xsl:attribute>
</xsl:template>

<!-- change the class keyword to a interface keyword -->
<xsl:template match="src:class/text()">
    <xsl:value-of select="str:replace(., 'interface', 'implementation')"/>
</xsl:template>

</xsl:stylesheet>
