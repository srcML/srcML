<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:dbg="http://www.srcML.org/debug">

<xsl:template name="dbg:replace">
      <xsl:if test="contains(., 'abc')">
          <xsl:call-template name="dbg:replace" select="." />
      </xsl:if>
</xsl:template>

</xsl:stylesheet>
