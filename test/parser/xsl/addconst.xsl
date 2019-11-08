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
    addconst.xsl

    Adds const specifier to parameters
-->

<xsl:import href="copy.xsl"/>

<xsl:template match="src:function[not(src:specifier='const')]/src:block">
    <specifier>const</specifier>
    <xsl:text> </xsl:text>
    <xsl:copy-of select="."/>
</xsl:template>

</xsl:stylesheet>
