#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file xpath_attribute_invalid.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# must specify attribute after xpath query
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:pre="foo.com" revision="REVISION">

	<unit revision="REVISION" language="C++"><expr_stmt><expr><name pre:attr="value">a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++"><expr_stmt><expr><name pre:attr="value">b</name></expr>;</expr_stmt>
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
srcml archive.xml --attribute="pre:attr=value" --xpath="//src:name"
check_exit 1

srcml --attribute="pre:attr=value" --xpath="//src:name" archive.xml
check_exit 1

# output to a file
srcml archive.xml --attribute="pre:attr=value" --xpath="//src:name" -o result.xml
check_exit 1

srcml --attribute="pre:attr=value" --xpath="//src:name" archive.xml -o result.xml
check_exit 1

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++"><expr_stmt><expr><name cpp:foo="test">a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$result"
createfile a.cpp "a;
"

# from a file
srcml a.cpp --attribute="cpp:foo=test" --xpath="//src:name"
check_exit 1

srcml --attribute="cpp:foo=test" --xpath="//src:name" a.cpp
check_exit 1

# from standard input
echo "a;" | srcml -l C++ --attribute="cpp:foo=test" --xpath="//src:name"
check_exit 1

# output to a file
srcml a.cpp --attribute="cpp:foo=test" --xpath="//src:name" -o result.xml
check_exit 1

srcml --attribute="cpp:foo=test" --xpath="//src:name" a.cpp -o result.xml
check_exit 1

echo "a;" | srcml -l C++ --attribute="cpp:foo=test" --xpath="//src:name" -o result.xml
check_exit 1
