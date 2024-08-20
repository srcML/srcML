#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file no_xml_decl_archive_single.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# no xml declaration on a single unit
define srcml <<- 'STDOUT'
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

define fsrcml <<- 'STDOUT'
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$srcml"
xmlcheck "$fsrcml"

createfile sub/a.cpp "a;"

# from standard in
srcml --no-xml-declaration -l C++ --archive < sub/a.cpp
check "$srcml"

srcml --no-xml-declaration --archive -l C++ < sub/a.cpp
check "$srcml"

srcml --archive --no-xml-declaration -l C++ < sub/a.cpp
check "$srcml"

srcml -l C++ --no-xml-declaration --archive < sub/a.cpp
check "$srcml"

srcml -l C++ --archive --no-xml-declaration < sub/a.cpp
check "$srcml"

srcml --archive -l C++ --no-xml-declaration < sub/a.cpp
check "$srcml"

srcml --no-xml-declaration -l C++ -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml --no-xml-declaration -o sub/a.cpp.xml -l C++ --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml -o sub/a.cpp.xml --no-xml-declaration -l C++ --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml -l C++ --no-xml-declaration -o sub/a.cpp.xml --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml -l C++ -o sub/a.cpp.xml --no-xml-declaration --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

srcml -o sub/a.cpp.xml -l C++ --no-xml-declaration --archive < sub/a.cpp
check sub/a.cpp.xml "$srcml"

# from a file
srcml sub/a.cpp --no-xml-declaration --archive
check "$fsrcml"

srcml --no-xml-declaration sub/a.cpp --archive
check "$fsrcml"

srcml sub/a.cpp --no-xml-declaration -o sub/a.cpp.xml --archive
check sub/a.cpp.xml "$fsrcml"

srcml --no-xml-declaration sub/a.cpp -o sub/a.cpp.xml --archive
check sub/a.cpp.xml "$fsrcml"

srcml --no-xml-declaration -o sub/a.cpp.xml sub/a.cpp --archive
check sub/a.cpp.xml "$fsrcml"

srcml sub/a.cpp -o sub/a.cpp.xml --no-xml-declaration --archive
check sub/a.cpp.xml "$fsrcml"

srcml -o sub/a.cpp.xml --no-xml-declaration sub/a.cpp --archive
check sub/a.cpp.xml "$fsrcml"

srcml -o sub/a.cpp.xml sub/a.cpp --no-xml-declaration --archive
check sub/a.cpp.xml "$fsrcml"
