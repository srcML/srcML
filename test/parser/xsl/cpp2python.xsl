<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
<!--
    @file cpp2python.xsl

    @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)

    Converts C++ to Python. Intended for small statement test cases.

    * if
    * while
    * return
    * break
    * continue
    * switch
    * empty statement
    * variable declaration
    * expression statement
-->

<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    xmlns:str="http://exslt.org/strings"
    exclude-result-prefixes="src cpp str"
    version="1.0">

<!-- Default copy -->
<xsl:import href="copy.xsl"/>

<!-- Convert the language to Python -->
<xsl:template match="src:unit/@language"><xsl:attribute name="language">Python</xsl:attribute></xsl:template>

<!-- <empty_stmt>;</empty_stmt> -> <pass>pass</pass> -->
<xsl:template match="src:empty_stmt"><pass>pass</pass></xsl:template>

<!-- <switch>switch ...</switch> -> <switch>match ...</match> -->
<xsl:template match="src:switch"><xsl:copy>match<xsl:value-of select="substring(text()[1],string-length('switch')+1)"/><xsl:apply-templates select="node()[position()!=1]"/></xsl:copy></xsl:template>

<!-- <default>default:</default> -> <case>case _:</case> -->
<xsl:template match="src:default"><case>case _<xsl:value-of select="substring(text()[1],string-length('default')+1)"/><xsl:apply-templates select="node()[position()!=1]"/></case></xsl:template>

<!-- Insert space if "if(" -->
<xsl:template match="text()[.='if' or .='while' or .='for']"><xsl:copy-of select="."/><xsl:text> </xsl:text></xsl:template>

<!-- return 1; -> return 1 -->
<xsl:template match="src:return[src:expr]"><xsl:copy><xsl:copy-of select="text()[1]"/><xsl:copy-of select="src:expr"/></xsl:copy></xsl:template>

<!-- return ; -> return  -->
<xsl:template match="src:return | src: continue | src:break"><xsl:copy><xsl:copy-of select="str:tokenize(text(),';')[1]/text()"/><xsl:copy-of select="src:expr"/></xsl:copy></xsl:template>

<!-- <condition>(<expr><name>a</name></expr>)</condition> -> <condition><expr><name>a</name></expr></condition> -->
<xsl:template match="src:condition"><xsl:copy><xsl:apply-templates select="src:expr | src:decl"/></xsl:copy></xsl:template>

<!-- <decl><type/> <name/> <init>= <expr><name/></expr></decl> -> <expr><name/> = <name/></expr> -->
<xsl:template match="src:decl[src:init]"><expr><xsl:apply-templates select="src:name"/> = <xsl:apply-templates select="src:init/src:expr/node()"/></expr></xsl:template>

<!-- <expr_stmt><expr><name>a</name></expr>;</expr_stmt> -> <expr_stmt><expr><name>a</name></expr></expr_stmt> -->
<xsl:template match="src:expr_stmt"><xsl:copy><xsl:copy-of select="src:expr"/></xsl:copy></xsl:template>

<!-- blocks -->
<!-- Note: Adds extra newline -->
<xsl:template match="src:block"><block>:<block_content><xsl:text>
</xsl:text><xsl:variable name="content"><xsl:apply-templates select="src:block_content/node()"/></xsl:variable><xsl:copy-of select="$content"/><xsl:text>
</xsl:text></block_content></block></xsl:template>

</xsl:stylesheet>
