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
    class2interface.xsl

    Converts srcML Java class definition to interface definition
-->

<xsl:import href="copy.xsl"/>

<xsl:template match="src:class">
    <interface>
        <xsl:apply-templates/>
    </interface>
</xsl:template>

<xsl:template match="src:class_decl">
    <interface_decl>
        <xsl:apply-templates/>
    </interface_decl>
</xsl:template>

<!-- change the class keyword to a interface keyword -->
<xsl:template match="src:class/text()|src:class_decl/text()">
    <xsl:value-of select="str:replace(., 'class', 'interface')"/>
</xsl:template>

</xsl:stylesheet>
