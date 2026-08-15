#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file cwd.sh
#
# @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

directory=$(pwd)

if [[ "$OSTYPE" == 'msys' || "$OSTYPE" == 'cygwin' ]]; then
    directory=$(pwd -W)
fi
# test
##
# check missingfile

defineXML srcml <<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0" url="">

	<unit revision="1.0.0" language="C++" filename="${directory}/a.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" filename="${directory}/b.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
STDOUT

createfile a.cpp "a;"
createfile b.cpp "b;"

srcml .
check "$srcml"
