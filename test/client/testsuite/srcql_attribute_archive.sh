#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file srcql_attribute_archive.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name pre:attr="value">a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name pre:attr="value">b</name></expr>;</expr_stmt>
	</unit>

	</unit>
  STDOUT

xmlcheck "$result"

createfile a.cpp "a;
"
createfile b.cpp "b;
"

srcml a.cpp b.cpp --xmlns:pre=foo.com -o archive.xml

# from the files
srcml archive.xml --srcql='$N' --attribute="pre:attr=value"
check "$result"

srcml --srcql='$N' archive.xml --attribute="pre:attr=value"
check "$result"

# output to a file
srcml archive.xml --srcql='$N' --attribute="pre:attr=value" -o result.xml
check result.xml "$result"

srcml --srcql='$N' archive.xml --attribute="pre:attr=value" -o result.xml
check result.xml "$result"
