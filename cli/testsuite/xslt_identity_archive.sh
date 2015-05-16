#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test xslt on archive of one
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define identity <<- 'STDOUT'
	<?xml version="1.0"?>
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
	  <xsl:template match="@* | node()">
	      <xsl:copy>
	         <xsl:apply-templates select="@* | node()"/>
	      </xsl:copy>
	   </xsl:template>
	</xsl:stylesheet>
	STDOUT

define output <<- 'STDOUT'
	xslt : identity.xsl
	STDOUT

createfile sub/archive_single.cpp.xml "$srcml"
createfile identity.xsl "$identity"

# simple copy
srcml sub/archive_single.cpp.xml --xslt=identity.xsl
check 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl sub/archive_single.cpp.xml
check 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl < sub/archive_single.cpp.xml
check 3<<< "$srcml" 4<<< "$output"

srcml sub/archive_single.cpp.xml --xslt=identity.xsl -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml sub/archive_single.cpp.xml -o sub/b.cpp.xml --xslt=identity.xsl
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl sub/archive_single.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl -o sub/b.cpp.xml sub/archive_single.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl -o sub/b.cpp.xml < sub/archive_single.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

# xslt apply root copy
srcml sub/archive_single.cpp.xml --apply-root --xslt=identity.xsl
check 3<<< "$srcml" 4<<< "$output"

srcml sub/archive_single.cpp.xml --xslt=identity.xsl --apply-root
check 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl sub/archive_single.cpp.xml
check 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl < sub/archive_single.cpp.xml
check 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl sub/archive_single.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl -o sub/b.cpp.xml sub/archive_single.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl -o sub/b.cpp.xml < sub/archive_single.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"


# test on archive of many
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

createfile sub/archive_multi.cpp.xml "$srcml"

# simple copy
srcml sub/archive_multi.cpp.xml --xslt=identity.xsl
check 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl sub/archive_multi.cpp.xml
check 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl < sub/archive_multi.cpp.xml
check 3<<< "$srcml" 4<<< "$output"

srcml sub/archive_multi.cpp.xml --xslt=identity.xsl -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml sub/archive_multi.cpp.xml -o sub/b.cpp.xml --xslt=identity.xsl
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl sub/archive_multi.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl -o sub/b.cpp.xml sub/archive_multi.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --xslt=identity.xsl -o sub/b.cpp.xml < sub/archive_multi.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

# xslt apply root copy
srcml sub/archive_multi.cpp.xml --apply-root --xslt=identity.xsl
check 3<<< "$srcml" 4<<< "$output"

srcml sub/archive_multi.cpp.xml --xslt=identity.xsl --apply-root
check 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl sub/archive_multi.cpp.xml
check 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl < sub/archive_multi.cpp.xml
check 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl sub/archive_multi.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl -o sub/b.cpp.xml sub/archive_multi.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

srcml --apply-root --xslt=identity.xsl -o sub/b.cpp.xml < sub/archive_multi.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "$output"

