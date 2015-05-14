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
	extractunit.xsl

	Extracts a specific imbedded unit

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:param name="number"/>

<xsl:output method="xml" omit-xml-declaration="yes"/>

<!--
<xsl:namespace-alias stylesheet-prefix="src" result-prefix=""/>
-->

<xsl:template match="/src:unit">
	<xsl:apply-templates select="src:unit"/>
</xsl:template>

<!-- filter out second-level units of the wrong position -->
<xsl:template match="/src:unit/src:unit">
  <xsl:if test="position()=$number">
<unit xmlns="http://www.srcML.org/srcML/src">
	<xsl:apply-templates/>
</unit>
  </xsl:if>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
