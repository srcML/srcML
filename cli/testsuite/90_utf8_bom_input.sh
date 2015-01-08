#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# UTF-8 BOM

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

src2srcml --language 'C' <<< "a;"

check 3<<< "$output"

message "\xef\xbb\xbfa;"

src2srcml --language 'C' <<< "\xef\xbb\xbfa;"

check 3<<< "$output"
