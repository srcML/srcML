#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define nestedfile <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="sub/b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDIN

# test invalid_input
echo "a;" | srcml -l C++ --strip
check_exit 1

# unknown encoding
echo "a;" | srcml -l C++ --src-encoding="ISO"
check_exit 1

echo "a;" | srcml -l C++ --xml-encoding="ISO"
check_exit 1

# source encoding not given
echo "a;" | srcml -l C++ --src-encoding
check_exit 1

# xml encoding not given
echo "a;" | srcml -l C++ --xml-encoding
check_exit 1

# invalid language
echo "a;" | srcml --language bad_language "foobar"
check_exit 1

# missing value
echo "a;" | srcml --language
check_exit 1

echo "a;" | srcml -l C++ --filename
check_exit 1

echo "a;" | srcml -l C++ --url
check_exit 1

echo "a;" | srcml -l C++ --src-version
check_exit 1

# invalid input filename
srcml foobar
check_exit 1

# unknown option
srcml --strip foobar
check_exit 1

# unknown encoding
srcml --src-encoding="ISO" --text="a;" -l C
check_exit 1

# source encoding not given
srcml  --text="a;" -l C --src-encoding
check_exit 1

# unit option selected but no value
srcml --unit
check_exit 1

# unit value too large
srcml --unit 3 <<< "$nestedfile"
check_exit 1

srcml --unit 3 -X <<< "$nestedfile"
check_exit 1

srcml --unit 3 --filename <<< "$nestedfile"
check_exit 1

srcml --unit 3 --url <<< "$nestedfile"
check_exit 1

srcml --unit 3 --src-version <<< "$nestedfile"
check_exit 1

# invalid combinations
srcml -X --src-encoding "UTF-8" "foobar"
check_exit 1
