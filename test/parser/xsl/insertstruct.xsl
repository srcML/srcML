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

<xsl:template match="src:unit[@language='C++'][src:expr_stmt//src:call/src:name/text()='~']" priority="9">
<xsl:copy>
<struct>struct <name>S</name> <block>{<public type="default"><xsl:text>
</xsl:text><comment type="block">/* <xsl:copy-of select="string(.)"/> */</comment><xsl:text>
</xsl:text></public>}</block>;</struct>
</xsl:copy>
</xsl:template>

<xsl:template match="src:unit[@language='C++'][src:expr_stmt]">
<xsl:copy>
<struct>struct <name>S</name> <block>{<public type="default"><xsl:text>
</xsl:text><block>{<block_content><xsl:copy-of select="*|text()"/></block_content>}</block><xsl:text>
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
