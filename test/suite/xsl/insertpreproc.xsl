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

<!--
	insertpreproc.xsl

	Inserts preprocessor statements at every opportunity

	Michael L. Collard
	collard@uakron.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8" standalone="yes"/>

<xsl:variable name="cmt" xml:space="preserve"><xsl:text>
</xsl:text><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define><xsl:text>
</xsl:text></xsl:variable> 

<!-- change url name -->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
		<xsl:value-of select="."/><xsl:text>.preproc</xsl:text>
	</xsl:attribute>
</xsl:template>

<!-- change the directory attribute on any unit from "if" to "while" -->
<xsl:template match="src:comment">
  <xsl:copy-of select="."/>
</xsl:template>

<xsl:template match="src:unit/src:unit//text()[contains(., ' ') and not(ancestor::src:literal)]">
  <xsl:apply-templates select="." mode="replace"/>
</xsl:template>

<xsl:template match="src:expr/text() |  src:type/text() ">
  <xsl:copy-of select="."/>
</xsl:template>

<xsl:template match="text()[not(parent::cpp:*)][contains(., ' ') and not(ancestor::src:literal)]" mode="replace">
  <xsl:value-of select="substring-before(., ' ')"/>
  <xsl:value-of select="' '"/>
  <xsl:copy-of select="$cmt"/>
  <xsl:value-of select="' '"/>
  <xsl:value-of select="substring-after(., ' ')"/>
<!--  <xsl:apply-templates select="substring-after(., ' ')" mode="replace"/> -->
</xsl:template>

<xsl:template match="text()" mode="replace">
  <xsl:value-of select="."/><xsl:value-of select="abc"/>
</xsl:template>

<!-- filter out comments -->
<xsl:template match="comment()"/>

<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
