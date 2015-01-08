#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# UTF-8 BOM

src_no_bom = STDOUTa;
	STDOUT
src_bom = STDOUT\xef\xbb\xbfa;
	STDOUT

	
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
		STDOUT

src2srcml --language 'C' src_no_bom srcml)
src2srcml --language 'C' src_bom srcml)

# xpath various return types

# attribute

