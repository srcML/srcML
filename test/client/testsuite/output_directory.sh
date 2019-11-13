#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
  STDOUT
createfile a.xml "$srcml"
createfile a.cpp "a;"

# output directory does not exist
rm -rf nonexistent_dir

echo "a;" | srcml -l C++ -o nonexistent_dir/a.xml --filename "a.cpp"
check nonexistent_dir/a.xml "$srcml"

rm -rf nonexistent_dir

srcml a.cpp -o nonexistent_dir/a.xml
check nonexistent_dir/a.xml "$srcml"

rm -rf nonexistent_dir

srcml a.xml -o nonexistent_dir/a.cpp
check nonexistent_dir/a.cpp "a;"
