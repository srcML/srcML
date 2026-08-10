#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file text_multiple.sh
#
# @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# each --text on the command line is a separate unit in a srcML archive

defineXML multiplesrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
STDOUT

srcml --text="a;" --text="b;" -l C++
check "$multiplesrcml"

srcml -l C++ --text="a;" --text="b;"
check "$multiplesrcml"

# short and long forms mixed
srcml -l C++ -t "a;" --text "b;"
check "$multiplesrcml"

srcml -l C++ --text "a;" -t "b;"
check "$multiplesrcml"

# more than two applications
defineXML multiple3srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="e8622977d7b817a78d262b7a3d222bee631740f8"><expr_stmt><expr><name>c</name></expr>;</expr_stmt></unit>

	</unit>
STDOUT

srcml -l C++ --text="a;" --text="b;" --text="c;"
check "$multiple3srcml"

srcml -l C++ -t "a;" -t "b;" -t "c;"
check "$multiple3srcml"

createfile sub/f1.cpp "a;"
createfile sub/f2.cpp "b;"

# every --text unit comes before every input filename, whatever the order used
defineXML textfilesrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" filename="sub/f1.cpp" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" filename="sub/f2.cpp" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
STDOUT

srcml -l C++ --text="a;" sub/f1.cpp --text="b;" sub/f2.cpp
check "$textfilesrcml"

srcml -l C++ --text="a;" --text="b;" sub/f1.cpp sub/f2.cpp
check "$textfilesrcml"

srcml -l C++ sub/f1.cpp sub/f2.cpp --text="a;" --text="b;"
check "$textfilesrcml"
