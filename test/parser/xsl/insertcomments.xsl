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

<!--
	insertcomments.xsl

	Inserts comments at every opportunity
-->

<xsl:import href="copy.xsl"/>

<xsl:variable name="cmt" xml:space="preserve"> <comment type="block">/* a */</comment> </xsl:variable> 

<xsl:template match="src:comment | src:literal">
  <xsl:copy-of select="."/>
</xsl:template>

<xsl:template match="text()[contains(., ' ')]"><xsl:value-of select="concat(substring-before(., ' '), $cmt, substring-after(., ' '))"/>
	<!--
  <xsl:value-of select="substring-before(., ' ')"/>
  <xsl:copy-of select="$cmt"/>
  <xsl:value-of select="substring-after(., ' ')"/>
-->
</xsl:template>

</xsl:stylesheet>
