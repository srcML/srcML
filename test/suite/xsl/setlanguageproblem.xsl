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
	setlanguage.xsl

	Converts srcML if statement test cases to while statement test cases

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="ISO-8859-1" standalone="yes"/>

<xsl:param name="language" select="'Java'"/>

<!-- trap the current language attribute -->
<!--
<xsl:template match="/src:unit">
  <unit>
    <xsl:copy-of select="@*[name()!='language']"/>
    <xsl:attribute name="language"><xsl:value-of select="$language"/></xsl:attribute>

    <xsl:apply-templates select="node()"/>
  </unit>
</xsl:template>
-->

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
