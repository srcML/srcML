#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file srcql_multiple.sh
#
# @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# each --srcql on the command line is a separate transformation, applied in order

defineXML decls <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" filename="sub/a.cpp" item="1"><decl><type><name>int</name></type> <name>a</name></decl></unit>

	<unit revision="1.0.0" language="C++" filename="sub/a.cpp" item="2"><decl><type><name>char</name></type> <name>b</name></decl></unit>

	</unit>
STDOUT

defineXML names <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" filename="sub/a.cpp" item="1"><name>int</name></unit>

	<unit revision="1.0.0" language="C++" filename="sub/a.cpp" item="2"><name>a</name></unit>

	<unit revision="1.0.0" language="C++" filename="sub/a.cpp" item="3"><name>char</name></unit>

	<unit revision="1.0.0" language="C++" filename="sub/a.cpp" item="4"><name>b</name></unit>

	</unit>
STDOUT

defineXML empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
STDOUT

createfile sub/a.cpp "int a;
char b;
"

srcml -l C++ sub/a.cpp -X -o sub/a.cpp.xml

# a single query, for comparison
srcml --srcql='FIND $T $V' sub/a.cpp.xml
check "$decls"

# repeated query, where the second matches everything the first found
srcml --srcql='FIND $T $V' --srcql='FIND $T $V' sub/a.cpp.xml
check "$decls"

srcml --srcql='FIND $T $V' --srcql='FIND $T $V' --srcql='FIND $T $V' sub/a.cpp.xml
check "$decls"

# separated form
srcml --srcql 'FIND $T $V' --srcql 'FIND $T $V' sub/a.cpp.xml
check "$decls"

# input filename before the queries
srcml sub/a.cpp.xml --srcql='FIND $T $V' --srcql='FIND $T $V'
check "$decls"

# input filename between the queries
srcml --srcql='FIND $T $V' sub/a.cpp.xml --srcql='FIND $T $V'
check "$decls"

# from standard input
srcml --srcql='FIND $T $V' --srcql='FIND $T $V' < sub/a.cpp.xml
check "$decls"

srcml --srcql='FIND $T $V' --srcql='FIND $T $V' sub/a.cpp.xml -o sub/b.xml
check sub/b.xml "$decls"

# the second query is applied to the results of the first, so it can match nothing
srcml --srcql='FIND $T $V' --srcql='FIND int $V' sub/a.cpp.xml
check "$empty"

# mixed with --xpath, in both orders
srcml --srcql='FIND $T $V' --xpath='//src:name' sub/a.cpp.xml
check "$names"

srcml --xpath='//src:decl' --srcql='FIND $T $V' sub/a.cpp.xml
check "$decls"
