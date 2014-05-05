<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:src="http://www.sdml.info/srcML/src"
	xmlns="http://www.sdml.info/srcML/src"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	xmlns:str="http://exslt.org/strings"
	xmlns:func="http://exslt.org/functions"
	xmlns:exsl="http://exslt.org/common"
	extension-element-prefixes="str exsl func"
	exclude-result-prefixes="src"
	version="1.0">

<!--
	for2while.xsl

	Converts srcML if statement test cases to while statement test cases

	Michael L. Collard
	collard@mcs.kent.edu
-->


<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8"/>

<!-- change the directory attribute on any unit from "if" to "while" -->
<xsl:template match="src:unit/@dir">
	<xsl:attribute name="dir">
		<xsl:value-of select="concat(., '.', $new_name)"/>
	</xsl:attribute>
</xsl:template>

<!-- change the if keyword to a while keyword -->
<xsl:template match="src:name/text()[1]">
	<xsl:value-of select="str:replace(., 'NAME', $new_name)"/>
</xsl:template>

<!-- change the filename attribute on the unit from "if" to "while" -->
<xsl:template match="src:unit/@filename">
	<xsl:attribute name="filename">
		<xsl:value-of select="concat(., '.', $new_name)"/>
	</xsl:attribute>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
