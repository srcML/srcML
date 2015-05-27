#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test xslt empty input
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define paramxslt <<- 'STDOUT'
	<xsl:stylesheet xmlns="http://www.srcML.org/srcML/src" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:cpp="http://www.srcML.org/srcML/cpp" version="1.0">
	<xsl:output method="text"/>
	<xsl:param name="paramname"/>
	<xsl:template match="/"><xsl:text select="$paramname"/></xsl:template>
	</xsl:stylesheet>
	STDOUT

createfile sub/a.cpp.xml "$srcml"
#cp sub/a.cpp.xml a.cpp.xml
createfile param.xsl "$paramxslt"
#cp param.xsl myparam.xsl

message "HELLOO2"

srcml --xslt=param.xsl --stringparam paramname='paramvalue' sub/a.cpp.xml
check 3<<< "stuff"

message "HELLOO3"
# functionality missing
