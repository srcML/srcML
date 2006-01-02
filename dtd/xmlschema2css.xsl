<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xs="http://www.w3.org/2001/XMLSchema"
	version="1.0">

<!--
        xmlschema2css.xsl

        XML Schema to CSS generator
	Copies from input to output

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:output method="text"/>

<xsl:template match="/">/*
  srcmltags.css

  CSS file for marking tags in styled views of srcML documents.
  Generated automatically from an XML Schema for srcML by xmlschema2css.xsl

  Michael L. Collard
  collard@cs.kent.edu
*/
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="xs:element"><xsl:variable name="tagname" select="@name"/>
/* tag <xsl:value-of select="$tagname"/> */
<xsl:apply-templates select="." mode="before"/>
<xsl:apply-templates select="." mode="after"/>
</xsl:template>

<xsl:template match="xs:element" mode="before"><xsl:variable name="tagname" select="@name"/>
<xsl:value-of select="$tagname" />::before {
  <xsl:apply-templates select="." mode="beforecontent"/>
}
</xsl:template>

<xsl:template match="xs:element[count(.//xs:enumeration)&gt;1]" mode="before"><xsl:variable name="tagname" select="@name"/>
  <xsl:apply-templates select="." mode="beforefirst"/>
  <xsl:apply-templates select="." mode="beforesecond"/>
</xsl:template>

<xsl:template match="xs:element" mode="beforefirst"><xsl:variable name="tagname" select="@name"/>
<xsl:value-of select="$tagname" />[]::before {
  <xsl:apply-templates select="." mode="beforecontentfirst"/>
}
</xsl:template>

<xsl:template match="xs:element" mode="beforesecond"><xsl:variable name="tagname" select="@name"/>
<xsl:value-of select="$tagname" />[]::before {
  <xsl:apply-templates select="." mode="beforecontentsecond"/>
}
</xsl:template>

<xsl:template match="xs:element" mode="beforecontent"><xsl:variable name="tagname" select="@name"/>
  content : "&lt;<xsl:value-of select="$tagname"/><xsl:apply-templates select=".//xs:attribute" mode="second"/>&gt;"
</xsl:template>

<xsl:template match="xs:element" mode="beforecontentfirst"><xsl:variable name="tagname" select="@name"/>
  content : "&lt;<xsl:value-of select="$tagname"/><xsl:apply-templates select=".//xs:attribute" mode="first"/>&gt;"
</xsl:template>

<xsl:template match="xs:element" mode="beforecontentfirst"><xsl:variable name="tagname" select="@name"/>
  content : "&lt;<xsl:value-of select="$tagname"/><xsl:apply-templates select=".//xs:attribute" mode="second"/>&gt;"
</xsl:template>

<xsl:template match="xs:element" mode="after"><xsl:variable name="tagname" select="@name"/>
<xsl:value-of select="$tagname" />::after {
  <xsl:apply-templates select="." mode="aftercontent"/>
}
</xsl:template>

<xsl:template match="xs:element" mode="aftercontent"><xsl:variable name="tagname" select="@name"/>
  content : "&lt;/<xsl:value-of select="$tagname"/>&gt;"
</xsl:template>

<xsl:template match="xs:attribute" mode="first">
<xsl:text> </xsl:text><xsl:value-of select="@name"/>="<xsl:value-of select="//xs:enumeration[1]/@value"/>"
</xsl:template>

<xsl:template match="xs:attribute" mode="second">
<xsl:text> </xsl:text><xsl:value-of select="@name"/>="<xsl:value-of select="//xs:enumeration[2]/@value"/>"
</xsl:template>

</xsl:stylesheet>