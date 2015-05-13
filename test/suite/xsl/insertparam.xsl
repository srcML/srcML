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
	insertparam.xsl

	Inserts expressions into statements

	Michael L. Collard
	collard@mcs.kent.edu
-->

<xsl:param name="decl_filename"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8"/>

<xsl:variable name="declarations" select="document($decl_filename)//src:decl_stmt[not(.//src:block) and not(.//src:call)]"/>

<!-- select the base examples -->
<xsl:template match="/src:unit/src:unit">

	<xsl:variable name="m" select="."/>

	<xsl:for-each select="$declarations">
		<xsl:apply-templates select="$m" mode="insert">
			<xsl:with-param name='newdecl' select="./*|(./text()[position()&lt;last()])"/>
		</xsl:apply-templates>

	</xsl:for-each>
</xsl:template>

<!-- select the examples with an expression -->
<xsl:template match="/src:unit/src:unit" mode="insert">
	<xsl:param name="newdecl"/>

	<xsl:text>
</xsl:text>
	<xsl:copy>
	<xsl:text>
</xsl:text>
		<xsl:apply-templates select="*" mode="insert">
			<xsl:with-param name="newdecl" select="$newdecl"/>
		</xsl:apply-templates>
	<xsl:text>
</xsl:text>
	</xsl:copy>
	<xsl:text>
</xsl:text>

</xsl:template>

<!-- select the examples with an expression -->
<xsl:template match="*" mode="insert">
	<xsl:param name="newdecl"/>

	<xsl:choose>
	<xsl:when test="name()='formal_params'">
	        <formal_params>(<parameter><xsl:copy-of select="$newdecl"/></parameter>)</formal_params>
	</xsl:when>
	<xsl:otherwise>
		<xsl:copy>
			<xsl:apply-templates select="node()|@*" mode="insert">
				<xsl:with-param name="newdecl" select="$newdecl"/>
			</xsl:apply-templates>
		</xsl:copy>
	</xsl:otherwise>
	</xsl:choose>

</xsl:template>

<!-- select the examples with an expression -->
<xsl:template match="@*" mode="insert">
  <xsl:apply-templates select="."/>
</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
