#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# files from
define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$nestedfile"

createfile sub/a.cpp "
a;"
createfile sub/b.cpp "
b;"

srcml --files-from "https://raw.githubusercontent.com/srcML/test-data/1.0.0/filelist/file-list.txt"
check "$nestedfile"

srcml --files-from "https://raw.githubusercontent.com/srcML/test-data/1.0.0/filelist/file-list.txt" -o sub/both.xml
check sub/both.xml "$nestedfile"

# compressed remote filelist
define empty_srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
	STDOUT

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.bz2
check "$empty_srcml_with_url"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.gz
check "$empty_srcml_with_url"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.bz2.gz
check "$empty_srcml_with_url"

srcml --files-from https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.txt.gz.bz2
check "$empty_srcml_with_url"
