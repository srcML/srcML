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
	if2while.xsl

	Converts srcML if statement test cases to while statement test cases

	Michael L. Collard
	collard@mcs.kent.edu
-->


<xsl:import href="str.replace.function.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8"/>

<func:function name="src:ifwithelse">
	<xsl:param name="context"/>

	<func:result select="boolean($context[src:if//src:else])"/>

</func:function>

<func:function name="src:ifwithelse2">
	<xsl:param name="context"/>

	<func:result select="$context[src:if//src:else]"/>

</func:function>

<!-- change the url attribute on any unit from "if" to "while" -->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
		<xsl:value-of select="str:replace(., 'if', 'while')"/>
	</xsl:attribute>
</xsl:template>

<!-- filter out any if statements with an else -->
<xsl:template match="*/src:ifwithelse2(.)"/>
<!-- <xsl:template match="src:unit[src:ifwithelse(.)]"/> -->

<!-- filter the then elements, but not the contents of the then -->
<xsl:template match="src:if/src:then">
	<xsl:apply-templates/>
</xsl:template>

<!-- change the if element to a while element -->
<xsl:template match="src:if[not(.//src:else)]">
	<while>
		<xsl:apply-templates select="*|@*|text()"/>
	</while>
</xsl:template>

<!-- change the if keyword to a while keyword -->
<xsl:template match="src:if/text()[1]">
	<xsl:value-of select="str:replace(., 'if', 'while')"/>
</xsl:template>

<!-- change the filename attribute on the unit from "if" to "while" -->
<xsl:template match="src:unit/@filename">
	<xsl:attribute name="filename">
		<xsl:value-of select="str:replace(., 'if', 'while')"/>
	</xsl:attribute>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
