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
    defn2decl.xsl

    Converts srcML function definitions to function declarations
-->

<xsl:import href="copy.xsl"/>

<!-- change the function element to a function declaration element -->
<xsl:template match="src:function">
    <function_decl>
        <xsl:apply-templates select="*|@*|text()"/>
    </function_decl>
</xsl:template>

<!-- change the if keyword to a while keyword -->
<xsl:template match="src:function/src:block">
    <xsl:text>;</xsl:text>
</xsl:template>

</xsl:stylesheet>
