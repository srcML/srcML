#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$output"

# input from a valid remote source file
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp
check "$output"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp -o remote.xml
check remote.xml "$output"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp --archive
check "$archive_output"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp	--archive -o remote_archive.xml
check remote_archive.xml "$archive_output"

# files from contains a remote source file
createfile list.txt "https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp"

srcml --files-from list.txt
check "$archive_output"

srcml --files-from list.txt -o remote_from_list.xml
check remote_from_list.xml "$archive_output"
