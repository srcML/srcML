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
    if2while.xsl

    Converts srcML if statement test cases to while statement test cases
-->

<xsl:import href="copy.xsl"/>

<!-- filter out any if statements with an else -->
<xsl:template match="src:unit[src:if_stmt//src:else]"/>

<!-- filter the if_stmt elements, but not the contents of the then -->
<xsl:template match="src:if_stmt">
    <xsl:apply-templates/>
</xsl:template>

<!-- change the if element to a while element -->
<xsl:template match="src:if[not(.//src:else)]">
    <while>
        <xsl:apply-templates select="*|@*|text()"/>
    </while>
</xsl:template>

<!-- change the if keyword to a while keyword -->
<xsl:template match="src:if/text()[1]">
    <xsl:value-of select="str:replace(., 'if', 'while')"/>
</xsl:template>

</xsl:stylesheet>
