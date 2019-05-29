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
    insertformfeed.xsl

    Inserts formfeeds at every opportunity
-->

<xsl:import href="insertcomment.xsl"/>

<xsl:variable name="cmt" xml:space="preserve"> <escape char="0x0c"/></xsl:variable> 

</xsl:stylesheet>
