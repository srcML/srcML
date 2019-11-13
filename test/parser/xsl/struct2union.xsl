<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    xmlns:str="http://exslt.org/strings"
    xmlns:func="http://exslt.org/functions"
    xmlns:exsl="http://exslt.org/common"
    extension-element-prefixes="str exsl func"
    exclude-result-prefixes="src cpp"
    version="1.0">

<!--
    struct2union.xsl

    Converts srcML struct statement test cases to union statement test cases
-->

<xsl:import href="copy.xsl"/>

<!-- change struct element to union element -->
<xsl:template match="src:struct">
    <union><xsl:apply-templates/></union>
</xsl:template>

<!-- change struct deleclaration to union declaration -->
<xsl:template match="src:struct_decl">
    <union_decl><xsl:apply-templates/></union_decl>
</xsl:template>

<!-- change the struct keyword to a union keyword -->
<xsl:template match="src:struct/text() | src:struct_decl/text()">
    <xsl:value-of select="str:replace(., 'struct', 'union')"/>
</xsl:template>

</xsl:stylesheet>
