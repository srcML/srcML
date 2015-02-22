#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test on compressed files with .zip.gz extension
define src <<- 'STDOUT'

	a;
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION" filename="file://archive/a.cpp.zip.gz">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="archive/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="archive/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile archive/a.cpp "$src"
zip archive/a.cpp.zip archive/a.cpp
gzip -c archive/a.cpp.zip > archive/a.cpp.zip.gz

src2srcml archive/a.cpp.zip.gz -o archive/a.cpp.xml
check archive/a.cpp.xml 3<<< "$foutput"

srcml archive/a.cpp.zip.gz
check 3<<< "$foutput"

srcml -l C++ < archive/a.cpp.zip.gz
check 3<<< "$output"

srcml -l C++ -o archive/a.cpp.xml < archive/a.cpp.zip.gz
check archive/a.cpp.xml 3<<< "$output"

rmfile archive/a.cpp
rmfile archive/a.cpp.zip
rmfile archive/a.cpp.zip.gz
