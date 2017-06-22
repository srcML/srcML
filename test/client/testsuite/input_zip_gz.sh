#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on compressed files with .zip.gz extension
define src <<- 'STDOUT'

	a;
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="archive/a.cpp.zip.gz">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="archive/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$archive_output"
xmlcheck "$foutput"
xmlcheck "$output"

createfile archive/a.cpp "$src"
zip archive/a.cpp.zip archive/a.cpp
gzip -c archive/a.cpp.zip > archive/a.cpp.zip.gz

createfile list.txt "archive/a.cpp.zip.gz"


# src --> srcml
src2srcml archive/a.cpp.zip.gz -o archive/a.cpp.xml
checkv2 archive/a.cpp.xml "$foutput"

src2srcml archive/a.cpp.zip.gz
checkv2 "$foutput"

src2srcml -l C++ < archive/a.cpp.zip.gz
checkv2 "$output"

src2srcml -l C++ -o archive/a.cpp.xml < archive/a.cpp.zip.gz
checkv2 archive/a.cpp.xml "$output"


# files from
src2srcml --files-from list.txt
checkv2 "$archive_output"

src2srcml --files-from list.txt -o archive/list.xml
checkv2 archive/list.xml "$archive_output"

# files from empty (not necessary - archive format)


rmfile list.txt
rmfile archive/a.cpp
rmfile archive/a.cpp.zip
rmfile archive/a.cpp.zip.gz


# srcml --> src
srcml2src archive/a.cpp.xml
checkv2 "$src"

srcml2src archive/a.cpp.xml -o archive/a.cpp
checkv2 archive/a.cpp "$src"

srcml2src < archive/a.cpp.xml
checkv2 "$src"

srcml2src -o archive/a.cpp < archive/a.cpp.xml
checkv2 archive/a.cpp "$src"
