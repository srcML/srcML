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
    class2struct.xsl

    Converts srcML class statement test cases to struct statement test cases
-->

<xsl:import href="copy.xsl"/>

<!-- change class element to struct element -->
<xsl:template match="src:class">
    <struct>
        <xsl:apply-templates/>
    </struct>
</xsl:template>

<!-- change class declaration element to struct declaration -->
<xsl:template match="src:class_decl">
    <struct_decl>
        <xsl:apply-templates/>
    </struct_decl>
</xsl:template>

<!-- change default public to default private -->
<xsl:template match="src:private[@type='default']">
    <public type="default">
        <xsl:apply-templates/>
    </public>
</xsl:template>

<!-- change the class keyword to a struct keyword -->
<xsl:template match="src:class/text() | src:class_decl/text()">
    <xsl:value-of select="str:replace(., 'class', 'struct')"/>
</xsl:template>

</xsl:stylesheet>
