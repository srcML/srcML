<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:str="http://exslt.org/strings"
                xmlns:func="http://exslt.org/functions"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="str exsl func">

<xsl:template name="str:_replace">
  <xsl:param name="string" select="''" />
  <xsl:param name="replacements" select="/.." />

      <xsl:variable name="replacement" select="$replacements[1]" />
      <xsl:variable name="search" select="$replacement/@search" />
      <xsl:choose>
        <xsl:when test="contains('a', 'abc')">
<!--        <xsl:when test="contains($string, $search)"> -->
        </xsl:when>

        <xsl:otherwise>
          <xsl:call-template name="str:_replace">
            <xsl:with-param name="string" select="$string" />
            <xsl:with-param name="replacements" select="$replacements[position() > 1]" />
          </xsl:call-template>
        </xsl:otherwise>

      </xsl:choose>

</xsl:template>

</xsl:stylesheet>
