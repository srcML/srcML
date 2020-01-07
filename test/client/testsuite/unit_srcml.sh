#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define nestedfile <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDIN

define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$nestedfile"
xmlcheck "$sxmlfile1"

srcml -X --unit "1" - <<< "$nestedfile"
check "$sxmlfile1"

srcml -X --unit "1" <<< "$nestedfile"
check "$sxmlfile1"

define sxmlfile2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define sxmlfile2fragment <<- 'STDOUT'
	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define sxmlfile2raw <<- 'STDOUT'

	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	STDOUT

define sxmlfile2src <<- 'STDOUT'

	b;
	STDOUT

# single newline
define emptysrc <<- 'STDOUT'
	STDOUT

xmlcheck "$sxmlfile2"

srcml -X --unit "2" - <<< "$nestedfile"
check "$sxmlfile2"

srcml --unit "2" --output-srcml-outer <<< "$nestedfile"
check "$sxmlfile2fragment"

srcml --unit "2" --output-srcml-inner <<< "$nestedfile"
check "$sxmlfile2raw"

srcml --text="\nb;\n" --filename="sub/b.cpp" -l C++ --hash | srcml
check "$sxmlfile2src"

srcml --text="" --filename="sub/b.cpp" -l C++ --hash | srcml
check "$emptysrc"

