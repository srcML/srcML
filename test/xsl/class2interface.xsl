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
	class2interface.xsl

	Converts srcML Java class definition to interface definition

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="ISO-8859-1"/>

<xsl:template match="/src:unit/text()[1]">

  <xsl:comment>Generated from class_java.cpp.xml by stylesheet class2interface.xsl</xsl:comment>
  <xsl:copy/>

</xsl:template>

<!-- change the directory attribute on any unit from "class" to "interface" -->
<xsl:template match="src:unit/@dir">
	<xsl:attribute name="dir">
		<xsl:value-of select="str:replace(., 'class', 'interface')"/>
	</xsl:attribute>
</xsl:template>

<!-- change the filename attribute on the unit from "class" to "interface" -->
<xsl:template match="src:unit/@filename">
	<xsl:attribute name="filename">
		<xsl:value-of select="str:replace(., 'class', 'interface')"/>
	</xsl:attribute>
</xsl:template>

<xsl:template match="src:class">
	<src:class type="interface">
		<xsl:apply-templates/>
	</src:class>
</xsl:template>

<xsl:template match="src:class_decl">
	<src:interface_decl>
		<xsl:apply-templates/>
	</src:interface_decl>
</xsl:template>

<xsl:template match="src:private[@type='default']">
	<src:public type="default">
		<xsl:apply-templates/>
	</src:public>
</xsl:template>

<!-- change the class keyword to a interface keyword -->
<xsl:template match="src:class/text()|src:class_decl/text()">
	<xsl:value-of select="str:replace(., 'class', 'interface')"/>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
