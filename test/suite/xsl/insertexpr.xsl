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
	insertexpr.xsl

	Inserts expressions into statements

	Michael L. Collard
	collard@mcs.kent.edu
-->

<xsl:param name="expr_filename"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="UTF-8"/>

<xsl:variable name="expressions" select="document($expr_filename)//src:expr"/>

<!-- select the examples with an expression -->
<xsl:template match="/src:unit/src:unit[.//src:expr]">

	<xsl:variable name="m" select="."/>

	<xsl:for-each select="$expressions">

		<xsl:apply-templates select="$m" mode="insert">
			<xsl:with-param name='newexpr' select="."/>
		</xsl:apply-templates>

	</xsl:for-each>
</xsl:template>

<!-- select the examples with an expression -->
<xsl:template match="/src:unit/src:unit" mode="insert">
	<xsl:param name="newexpr"/>

	<xsl:text>
</xsl:text>
	<xsl:copy>
	<xsl:text>
</xsl:text>
		<xsl:apply-templates select="*" mode="insert">
			<xsl:with-param name="newexpr" select="$newexpr"/>
		</xsl:apply-templates>
	<xsl:text>
</xsl:text>
	</xsl:copy>
	<xsl:text>
</xsl:text>

</xsl:template>

<!-- select the examples with an expression -->
<xsl:template match="*" mode="insert">
	<xsl:param name="newexpr"/>

	<xsl:choose>
	<xsl:when test="name()='expr'">
		<xsl:copy-of select="$newexpr"/>
	</xsl:when>
	<xsl:otherwise>
		<xsl:copy>
			<xsl:apply-templates select="node()" mode="insert">
				<xsl:with-param name="newexpr" select="$newexpr"/>
			</xsl:apply-templates>
		</xsl:copy>
	</xsl:otherwise>
	</xsl:choose>

</xsl:template>

<!-- default identity copy -->
<xsl:template match="@*|node()">
	<xsl:copy>
	  <xsl:apply-templates select="@*|node()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
