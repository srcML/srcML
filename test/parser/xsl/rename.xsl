<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    xmlns:str="http://exslt.org/strings"
    extension-element-prefixes="str"
    exclude-result-prefixes="src"
    version="1.0">

<!--
    rename.xsl

    Rename a default variable NAME to a given passed in name.
-->

<xsl:import href="copy.xsl"/>

<xsl:param name="new_name"/>

<!-- put in text identifier name -->
<xsl:template match="src:name/text()[1] | src:specifier/text()[1]">
    <xsl:value-of select="str:replace(., 'NAME', $new_name)"/>
</xsl:template>

</xsl:stylesheet>
