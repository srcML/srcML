#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file xmlns_multiple.sh
#
# @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# each --xmlns on the command line declares a separate namespace, where the
# position of the input filenames does not matter

defineXML foosrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<bar:unit xmlns:bar="http://www.srcML.org/srcML/src" xmlns="http://www.foo.com" revision="REVISION">

	<bar:unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><bar:expr_stmt><bar:expr><bar:name>a</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	<bar:unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><bar:expr_stmt><bar:expr><bar:name>b</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	</bar:unit>
STDOUT

defineXML foocppsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<bar:unit xmlns:bar="http://www.srcML.org/srcML/src" xmlns="http://www.foo.com" revision="REVISION">

	<bar:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><bar:expr_stmt><bar:expr><bar:name>a</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	<bar:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><bar:expr_stmt><bar:expr><bar:name>b</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	</bar:unit>
STDOUT

defineXML prefixsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<bar:unit xmlns:bar="http://www.srcML.org/srcML/src" revision="REVISION">

	<bar:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><bar:expr_stmt><bar:expr><bar:name>a</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	<bar:unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><bar:expr_stmt><bar:expr><bar:name>b</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	</bar:unit>
STDOUT

defineXML emptydefaultsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<bar:unit xmlns:bar="http://www.srcML.org/srcML/src" xmlns="" revision="REVISION">

	<bar:unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><bar:expr_stmt><bar:expr><bar:name>a</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	<bar:unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><bar:expr_stmt><bar:expr><bar:name>b</bar:name></bar:expr>;</bar:expr_stmt></bar:unit>

	</bar:unit>
STDOUT

createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

srcml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp
check "$foosrcml"

# the declaration order of the default and the prefix does not matter
srcml --xmlns:bar="http://www.srcML.org/srcML/src" --xmlns="http://www.foo.com" sub/a.cpp sub/b.cpp
check "$foosrcml"

# an input filename between the declarations
srcml --xmlns="http://www.foo.com" sub/a.cpp --xmlns:bar="http://www.srcML.org/srcML/src" sub/b.cpp
check "$foosrcml"

srcml --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp --xmlns="http://www.foo.com" sub/b.cpp
check "$foosrcml"

srcml --xmlns="http://www.foo.com" sub/a.cpp --xmlns:bar="http://www.srcML.org/srcML/src" sub/b.cpp -o sub/a.xml
check sub/a.xml "$foosrcml"

# the URI as a separate argument, instead of after an '='
srcml --xmlns "http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp
check "$foosrcml"

srcml --xmlns "http://www.foo.com" --xmlns "bar=http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp
check "$foosrcml"

srcml --xmlns "http://www.foo.com" sub/a.cpp --xmlns "bar=http://www.srcML.org/srcML/src" sub/b.cpp
check "$foosrcml"

srcml sub/a.cpp sub/b.cpp --xmlns "http://www.foo.com" --xmlns "bar=http://www.srcML.org/srcML/src"
check "$foosrcml"

# more than two declarations
srcml --xmlns="http://www.foo.com" --xmlns:bar="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp sub/b.cpp
check "$foocppsrcml"

srcml --xmlns="http://www.foo.com" sub/a.cpp --xmlns:bar="http://www.srcML.org/srcML/src" sub/b.cpp --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check "$foocppsrcml"

# only prefixes, with no default namespace
srcml --xmlns:bar="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp sub/b.cpp
check "$prefixsrcml"

# an empty default namespace alongside a prefix
srcml --xmlns= --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp
check "$emptydefaultsrcml"

srcml --xmlns="" --xmlns:bar="http://www.srcML.org/srcML/src" sub/a.cpp sub/b.cpp
check "$emptydefaultsrcml"

srcml --xmlns:bar="http://www.srcML.org/srcML/src" --xmlns= sub/a.cpp sub/b.cpp
check "$emptydefaultsrcml"
