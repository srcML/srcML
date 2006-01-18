<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.sdml.info/srcML/src"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	xmlns:str="http://exslt.org/strings"
	xmlns:func="http://exslt.org/functions"
	xmlns:exsl="http://exslt.org/common"
	extension-element-prefixes="str exsl func"
	exclude-result-prefixes="src"
	version="1.0">

<!--
	struct2union.xsl

	Converts srcML struct statement test cases to union statement test cases

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8"/>

<xsl:template match="/src:unit/text()[1]">

  <xsl:comment>Generated from struct.cpp.xml by stylesheet struct2union.xsl</xsl:comment>
  <xsl:copy/>

</xsl:template>

<!-- change the directory attribute on any unit from "struct" to "union" -->
<xsl:template match="src:unit/@dir">
	<xsl:attribute name="dir">
		<xsl:value-of select="str:replace(., 'struct', 'union')"/>
	</xsl:attribute>
</xsl:template>

<!-- change the filename attribute on the unit from "struct" to "union" -->
<xsl:template match="src:unit/@filename">
	<xsl:attribute name="filename">
		<xsl:value-of select="str:replace(., 'struct', 'union')"/>
	</xsl:attribute>
</xsl:template>

<!-- change the if element to a while element -->
<xsl:template match="src:struct">
	<src:union>
		<xsl:apply-templates/> <!-- select="*|@*|text()"/> -->
	</src:union>
</xsl:template>

<!-- change the struct keyword to a union keyword -->
<xsl:template match="src:struct/text()[1]">
	<xsl:value-of select="str:replace(., 'struct', 'union')"/>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
