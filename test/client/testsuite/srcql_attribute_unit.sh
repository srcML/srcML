#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file srcql_attribute_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define resultstdin <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION" language="C++"><expr_stmt><expr pre:attr="value"><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$resultstdin"

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr pre:attr="value"><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

# test setting the attribute on xpath query results
define resultnop <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$resultstdin"
xmlcheck "$result"
createfile a.cpp "a;
"

# from a file
srcml a.cpp --srcql='$N' --xmlns:pre=foo.com --attribute="pre:attr=value"
check "$result"

srcml --srcql='$N' a.cpp --xmlns:pre=foo.com --attribute="pre:attr=value"
check "$result"

srcml --srcql="n" a.cpp --xmlns:pre=foo.com --attribute="pre:attr=value"
check "$resultnop"

# from standard input
echo "a;" | srcml -l C++ --srcql='$N' --xmlns:pre=foo.com --attribute="pre:attr=value"
check "$resultstdin"

# output to a file
srcml a.cpp --srcql='$N' --xmlns:pre=foo.com --attribute="pre:attr=value" -o result.xml
check result.xml "$result"

srcml --srcql='$N' a.cpp --xmlns:pre=foo.com --attribute="pre:attr=value" -o result.xml
check result.xml "$result"

echo "a;" | srcml -l C++ --srcql='$N' --xmlns:pre=foo.com --attribute="pre:attr=value" -o result.xml
check result.xml "$resultstdin"
