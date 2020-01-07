#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# direct input from a remote source, with various compressions
define srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="test" filename="test"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

# compression only
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.bz2 --url="test" --filename="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.gz --url="test" --filename="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.bz2.gz --url="test" --filename="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.gz.bz2 --url="test" --filename="test"
check "$srcml_with_url"

# empty direct remote source input with various compressions
define empty_srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="test" filename="test"/>
	STDOUT

# compressed
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.bz2 --url="test" --filename="test"
check "$empty_srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.gz --url="test" --filename="test"
check "$empty_srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.bz2.gz --url="test" --filename="test"
check "$empty_srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/empty/empty.cpp.gz.bz2 --url="test" --filename="test"
check "$empty_srcml_with_url"
