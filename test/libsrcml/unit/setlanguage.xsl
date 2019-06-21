<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    version="1.0">

<!--
    setlanguage.xsl

    Converts srcML if statement test cases to while statement test cases

    Michael L. Collard
    collard@cs.kent.edu

    Modified for testing by:
    Michael John Decker
    mdecker6@kent.edu
-->

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<xsl:param name="language" select="'Java'"/>

<xsl:template match="/src:unit/@language">
    <xsl:attribute name="language"><xsl:value-of select="$language"/></xsl:attribute>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
</xsl:template>

</xsl:stylesheet>
