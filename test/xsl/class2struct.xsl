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
	class2struct.xsl

	Converts srcML class statement test cases to struct statement test cases

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8"/>

<xsl:template match="/src:unit/text()[1]">

  <xsl:comment>Generated from class.cpp.xml by stylesheet class2struct.xsl</xsl:comment>
  <xsl:copy/>

</xsl:template>

<!-- change the directory attribute on any unit from "class" to "struct" -->
<xsl:template match="src:unit/@dir">
	<xsl:attribute name="dir">
		<xsl:value-of select="str:replace(., 'class', 'struct')"/>
	</xsl:attribute>
</xsl:template>

<!-- change the filename attribute on the unit from "class" to "struct" -->
<xsl:template match="src:unit/@filename">
	<xsl:attribute name="filename">
		<xsl:value-of select="str:replace(., 'class', 'struct')"/>
	</xsl:attribute>
</xsl:template>

<!-- change the if element to a while element -->
<xsl:template match="src:class">
	<src:struct>
		<xsl:apply-templates/> <!-- select="*|@*|text()"/> -->
	</src:struct>
</xsl:template>

<!-- change the if element to a while element -->
<xsl:template match="src:class_decl">
	<src:struct_decl>
		<xsl:apply-templates/> <!-- select="*|@*|text()"/> -->
	</src:struct_decl>
</xsl:template>

<!-- change the if element to a while element -->
<xsl:template match="src:private[@type='default']">
	<src:public type="default">
		<xsl:apply-templates/> <!-- select="*|@*|text()"/> -->
	</src:public>
</xsl:template>

<!-- change the class keyword to a struct keyword -->
<xsl:template match="src:class/text()[1]|src:class_decl/text()[1]">
	<xsl:value-of select="str:replace(., 'class', 'struct')"/>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
