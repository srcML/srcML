#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# text flag with empty input
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"/>
	STDOUT

xmlcheck "$srcml"

printf "" | srcml -l C++
check "$srcml"

# simple input
define asrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

# input with ?
define srcml_question <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><ternary><condition><expr><name>a</name></expr> ?</condition><then> <expr><name>b</name></expr> </then><else>: <expr><name>c</name></expr></else></ternary></expr>;</expr_stmt></unit>
	STDOUT

xmlcheck "$asrcml"

printf "a;" | srcml -l "C++"
check "$asrcml"

printf "a ? b : c;" | srcml -l "C++"
check "$srcml_question"

# escaped newline
define ansrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$ansrcml"

printf "a;\n" | srcml -l "C++"
check "$ansrcml"

##
# Test archive creation using null-separated stdin

define absrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$absrcml"

define absrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

printf "a;\0b;" | srcml -l "C++"
check "$absrcml"

define abesrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

printf "a;\0b;\0" | srcml -l C++
check "$abesrcml"

define abeesrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	<unit revision="1.0.0" language="C++" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
	STDOUT

printf "a;\0b;\0\0" | srcml -l C++
check "$abeesrcml"

define eabsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	<unit revision="1.0.0" language="C++" hash="a301d91aac4aa1ab4e69cbc59cde4b4fff32f2b8"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="1.0.0" language="C++" hash="9a1e1d3d0e27715d29bcfbf72b891b3ece985b36"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

printf "\0a;\0b;" | srcml -l C++
check "$eabsrcml"

##
# Series of tests for --text escape. The full set of echo escape should be supported
# so tests are compared to output of echo
# Note that BSD echo (macOS) supports the -e escape, but does not have it listed
# on the man page

# escaped \a
printf "\aa;\a" | srcml -l C++ | srcml
check "\aa;\a"

# escaped \b
printf "\bb;\b" | srcml -l C++ | srcml
check "\bb;\b"

# escaped \f
printf "\ff;\f" | srcml -l C++ | srcml
check "\ff;\f"

# escaped \t
printf "\tt;\t" | srcml -l C++ | srcml
check "\tt;\t"

# escaped \v
printf "\vv;\v" | srcml -l C++ | srcml
check "\vv;\v"

# hex characters
printf "\x68;" | srcml -l C++ | srcml
check "\x68;"

printf "\010;" | srcml -l C++ | srcml
check "\010;"

# octal characters
# printf "\0150;" | srcml -l C++ | srcml
# check "\0150;"

printf "\03;" | srcml -l C++ | srcml
check "\03;"

# escaped \e
printf "\ee;\e" | srcml -l C++ | srcml
check "\x1be;\x1b"

# escaped \r
printf "\rr;" | srcml -l C++ | srcml
check "\nr;"
