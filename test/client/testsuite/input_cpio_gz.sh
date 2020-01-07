#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on compressed files with .cpio.gz extension
define src <<- 'STDOUT'

	a;
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="archive/a.cpp.cpio.gz">

	<unit revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$archive_output"
xmlcheck "$foutput"
xmlcheck "$output"

createfile archive/a.cpp "$src"
echo archive/a.cpp | tr " " "\n" | cpio -o > archive/a.cpp.cpio
gzip -c archive/a.cpp.cpio > archive/a.cpp.cpio.gz

createfile list.txt "archive/a.cpp.cpio.gz"

# src --> srcml
srcml archive/a.cpp.cpio.gz -o archive/a.cpp.xml
check archive/a.cpp.xml "$foutput" "1 block
"

srcml archive/a.cpp.cpio.gz
check "$foutput"

srcml -l C++ < archive/a.cpp.cpio.gz
check "$output"

srcml -l C++ -o archive/a.cpp.xml < archive/a.cpp.cpio.gz
check archive/a.cpp.xml "$output"

# files from
srcml --files-from list.txt
check "$archive_output"

srcml --files-from list.txt -o archive/list.xml
check archive/list.xml "$archive_output"

# files from empty (not necessary - archive format)

rmfile list.txt
rmfile archive/a.cpp
rmfile archive/a.cpp.cpio
rmfile archive/a.cpp.cpio.gz

# srcml --> src
srcml archive/a.cpp.xml
check "$src"

srcml archive/a.cpp.xml -o archive/a.cpp
check archive/a.cpp "$src"

srcml < archive/a.cpp.xml
check "$src"

srcml -o archive/a.cpp < archive/a.cpp.xml
check archive/a.cpp "$src"
