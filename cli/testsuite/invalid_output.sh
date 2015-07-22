#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh
set +e

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT
createfile a.xml "$srcml"
createfile a.cpp "a;"

# output directory does not exist
rm -rf nonexistent_dir

echo "a;" | srcml -l C++ -o nonexistent_dir/a.xml
check_exit 1

srcml a.cpp -o nonexistent_dir/a.xml
check_exit 1

srcml a.xml -o nonexistent_dir/a.cpp
check_exit 1

