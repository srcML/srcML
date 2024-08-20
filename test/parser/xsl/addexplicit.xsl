<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
<!--
    @file addexplicit.xsl

    @copyright Copyright (C) 2019-2024 srcML, LLC. (www.srcML.org)

    Adds explicit specifier to constructors
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
    exclude-result-prefixes="src"
    version="1.0">

<xsl:import href="copy.xsl"/>

<!-- filter the then elements, but not the contents of the then -->
<xsl:template match="src:constructor" xml:space="preserve"><constructor><specifier>explicit</specifier><xsl:text> </xsl:text><xsl:apply-templates/></constructor></xsl:template>

</xsl:stylesheet>
