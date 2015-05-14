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
	interface2implementation.xsl

	Converts srcML Objective-C class interface to class implementation

	Modified from class2interface.xsl written by:
	Michael L. Collard
	collard@cs.kent.edu

	Michael John Decker
	mdecker6@kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8"/>

<xsl:template match="/src:unit/text()[1]">

  <xsl:comment>Generated from class_interface_m.m.xml by stylesheet interface2implementation.xsl</xsl:comment>
  <xsl:copy/>

</xsl:template>

<!-- change the filename attribute on the unit from "class" to "interface" -->
<xsl:template match="src:unit/@filename">
	<xsl:attribute name="filename">
		<xsl:value-of select="str:replace(., 'interface', 'implementation')"/>
	</xsl:attribute>
</xsl:template>

<!-- change the class keyword to a interface keyword -->
<xsl:template match="src:class/@type">
	<xsl:attribute name="type">@implementation</xsl:attribute>
</xsl:template>

<!-- change the class keyword to a interface keyword -->
<xsl:template match="src:class/text()">
	<xsl:value-of select="str:replace(., 'interface', 'implementation')"/>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
