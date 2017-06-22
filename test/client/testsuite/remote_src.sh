#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="raw.githubusercontent.com/srcML/test-data/master/source/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="0.9.5">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="0.9.5" language="C++" filename="raw.githubusercontent.com/srcML/test-data/master/source/a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$output"


# input from a valid remote source file
srcml https://raw.githubusercontent.com/srcML/test-data/master/source/a.cpp
checkv2 "$output"

srcml https://raw.githubusercontent.com/srcML/test-data/master/source/a.cpp -o remote.xml
checkv2 remote.xml "$output"

srcml https://raw.githubusercontent.com/srcML/test-data/master/source/a.cpp --archive
checkv2 "$archive_output"

srcml https://raw.githubusercontent.com/srcML/test-data/master/source/a.cpp	--archive -o remote_archive.xml
checkv2 remote_archive.xml "$archive_output"


# files from contains a remote source file
createfile list.txt "https://raw.githubusercontent.com/srcML/test-data/master/source/a.cpp"

srcml --files-from list.txt
checkv2 "$archive_output"

srcml --files-from list.txt -o remote_from_list.xml
checkv2 remote_from_list.xml "$archive_output"
