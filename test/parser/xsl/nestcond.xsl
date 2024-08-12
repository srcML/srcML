<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
<!--
    @file nestcond.xsl

    @copyright Copyright (C) 2019-2024 srcML, LLC. (www.srcML.org)

    Replaces conditions with conditions with extra parentheses
-->

<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    exclude-result-prefixes="src"
    version="1.0">

<xsl:import href="copy.xsl"/>

<xsl:template match="src:condition/src:expr">
    <xsl:copy>
        <operator>(</operator>
        <xsl:apply-templates/>
        <operator>)</operator>
    </xsl:copy>
</xsl:template>

</xsl:stylesheet>
