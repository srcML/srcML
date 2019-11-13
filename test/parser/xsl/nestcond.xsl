<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    exclude-result-prefixes="src"
    version="1.0">

<!--
    nestcond.xsl

    Replaces conditions with conditions with extra parentheses
-->

<xsl:import href="copy.xsl"/>

<xsl:template match="src:condition/src:expr">
    <xsl:copy>
        <operator>(</operator>
        <xsl:apply-templates/>
        <operator>)</operator>
    </xsl:copy>
</xsl:template>

</xsl:stylesheet>
