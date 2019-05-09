<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    exclude-result-prefixes="src"
    version="1.0">

<!--
    insertstruct.xsl

    Inserts a struct around the contents in each unit
-->

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<xsl:template match="src:unit[@language='C++']">
<xsl:copy>
<struct>struct <name>S</name> <block>{<public type="default"><xsl:text>
</xsl:text><xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text></public>}</block>;</struct>
</xsl:copy>
</xsl:template>

<xsl:template match="src:unit[@language='C#']">
<xsl:copy>
<struct>struct <name>S</name> <block>{<xsl:text>
</xsl:text><xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text>}</block></struct>
</xsl:copy>
</xsl:template>

<xsl:template match="src:unit">
<xsl:copy>
<struct>struct <name>S</name> <block>{<xsl:text>
</xsl:text><xsl:copy-of select="*|text()"/><xsl:text>
</xsl:text>}</block>;</struct>
</xsl:copy>
</xsl:template>

</xsl:stylesheet>
