#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# UTF-8 BOM

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="bomfile.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$output"
xmlcheck "$foutput"

srcml --language 'C' <<< "a;"

check "$output"
createfile bomfile.c "\xef\xbb\xbfa;
"

srcml bomfile.c
check "$foutput"

# NOTE: This is broken in libsrcml
srcml --language 'C' < bomfile.c
check "$output"
