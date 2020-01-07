#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on compressed files with .bz2 extension
define src <<- 'STDOUT'

	a;
	STDOUT

define empty_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="archive/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
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
xmlcheck "$empty_output"

createfile archive/a.cpp "$src"
bzip2 -c archive/a.cpp > archive/a.cpp.bz2

createfile list.txt "archive/a.cpp.bz2"
bzip2 -c list.txt > list.txt.bz2

createfile empty.txt " "
bzip2 -c empty.txt > empty.txt.bz2

# src --> srcml
srcml archive/a.cpp.bz2 -o archive/a.cpp.xml
check archive/a.cpp.xml "$foutput"

srcml archive/a.cpp.bz2
check "$foutput"

srcml -l C++ < archive/a.cpp.bz2
check "$output"

srcml -l C++ -o archive/a.cpp.xml < archive/a.cpp.bz2
check archive/a.cpp.xml "$output"

# files from
srcml --files-from list.txt
check "$archive_output"

srcml --files-from list.txt.bz2
check "$archive_output"

srcml --files-from list.txt -o archive/list.xml
check archive/list.xml "$archive_output"

srcml --files-from list.txt.bz2 -o archive/compressed_list.xml
check archive/compressed_list.xml "$archive_output"

# files from empty
srcml --files-from empty.txt
check "$empty_output"

srcml --files-from empty.txt.bz2
check "$empty_output"

srcml --files-from empty.txt -o archive/empty.xml
check archive/empty.xml "$empty_output"

srcml --files-from empty.txt.bz2 -o archive/compressed_empty.xml
check archive/compressed_empty.xml "$empty_output"

rmfile list.txt
rmfile list.txt.bz2
rmfile empty.txt
rmfile empty.txt.bz2
rmfile archive/a.cpp
rmfile archive/a.cpp.bz2

# srcml --> src
srcml archive/a.cpp.xml
check "$src"

srcml archive/a.cpp.xml -o archive/a.cpp
check archive/a.cpp "$src"

srcml < archive/a.cpp.xml
check "$src"

srcml -o archive/a.cpp < archive/a.cpp.xml
check archive/a.cpp "$src"
