#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file suffix_file.sh
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test a line suffix, e.g., "n.cpp:2", on an input filename

define src <<- 'STDOUT'
	n1;
	n2;
	n3;
STDOUT

defineXML wholefile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="n.cpp"><expr_stmt><expr><name>n1</name></expr>;</expr_stmt>
	<expr_stmt><expr><name>n2</name></expr>;</expr_stmt>
	<expr_stmt><expr><name>n3</name></expr>;</expr_stmt>
	</unit>
STDOUT

defineXML secondline <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="n.cpp"><expr_stmt><expr><name>n2</name></expr>;</expr_stmt>
	</unit>
STDOUT

createfile n.cpp "$src"

# first, middle, and last line
srcml n.cpp:1 --output-src
check "n1;\n"

srcml n.cpp:2 --output-src
check "n2;\n"

srcml n.cpp:3 --output-src
check "n3;\n"

# a line suffix only changes the source, not the unit
srcml n.cpp:2
check "$secondline"

# no suffix is the entire file
srcml n.cpp
check "$wholefile"

# the filename extension is still used for the language
srcml n.cpp:2 --show-language
check "C++\n"

# a column starts the line at that position
srcml n.cpp:2:1 --output-src
check "n2;\n"

srcml n.cpp:2:2 --output-src
check "2;\n"

defineXML secondlinecolumn <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="n.cpp"><expr_stmt><expr><literal type="number">2</literal></expr>;</expr_stmt>
	</unit>
STDOUT

srcml n.cpp:2:2
check "$secondlinecolumn"

# the last column of the line
srcml n.cpp:2:3 --output-src
check ";\n"

# a column past the last column of the line starts at the end of the line
defineXML blankline <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="n.cpp">
	</unit>
STDOUT

srcml n.cpp:2:4 --output-src
check "\n"

srcml n.cpp:2:4
check "$blankline"

srcml n.cpp:2:9 --output-src
check "\n"

# the last line of a file with no line terminator
createfile m.cpp "a1;\na2;"

srcml m.cpp:2 --output-src
check "a2;\n"

# a suffix on a compressed file
createfile g.cpp "$src"
gzip -f g.cpp

srcml g.cpp.gz:2 --output-src
check "n2;\n"

# a suffix on each input file of an archive
defineXML archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="n.cpp" hash="4c867350266d08ee9d4d96a58e4069797812f83c"><expr_stmt><expr><name>n2</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="m.cpp" hash="2abeb44dccef9c1bfa06c76db4d076c1a48626d5"><expr_stmt><expr><name>a1</name></expr>;</expr_stmt>
	</unit>

	</unit>
STDOUT

srcml n.cpp:2 m.cpp:1
check "$archive"

# a line past the end of the file has no source
defineXML empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="n.cpp"/>
STDOUT

srcml n.cpp:9 --output-src
check ""

srcml n.cpp:9
check "$empty"

srcml n.cpp:9:1 --output-src
check ""

# a zero line or column is part of the filename
srcml n.cpp:2:0 --output-src
check_exit 1

srcml n.cpp:0:2 --output-src
check_exit 1

# a non-numeric suffix is part of the filename
srcml n.cpp:abc --output-src
check_exit 1

# a suffix too large for a line number is part of the filename
srcml n.cpp:99999999999999 --output-src
check_exit 1

# a suffix on a file that does not exist
srcml nonexistent.cpp:1 --output-src
check_exit 1
