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

	Michael L. Collard
	collard@cs.kent.edu
-->

<xsl:import href="insertcomments.xsl"/>

<xsl:output method="xml" omit-xml-declaration="no" version="1.0" encoding="ISO-8859-1" standalone="yes"/>

<xsl:variable name="cmt" xml:space="preserve"> <escape char="0xc"/></xsl:variable> 

<!-- change url name -->
<xsl:template match="src:unit/@url">
	<xsl:attribute name="url">
		<xsl:value-of select="."/><xsl:text>.formfeed</xsl:text>
	</xsl:attribute>
</xsl:template>

</xsl:stylesheet>
