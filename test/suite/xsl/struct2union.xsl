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

<!-- change the url attribute on any unit from "struct" to "union" -->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
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
	<union>
		<xsl:apply-templates/> <!-- select="*|@*|text()"/> -->
	</union>
</xsl:template>

<!-- change the if element to a while element -->
<xsl:template match="src:struct_decl">
	<union_decl>
		<xsl:apply-templates/> <!-- select="*|@*|text()"/> -->
	</union_decl>
</xsl:template>

<!-- change the struct keyword to a union keyword -->
<xsl:template match="text()[contains(., 'struct')]">
	<xsl:value-of select="str:replace(., 'struct', 'union')"/>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
