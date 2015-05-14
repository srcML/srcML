#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# UTF-8 BOM

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="bomfile.c"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

src2srcml --language 'C' <<< "a;"

check 3<<< "$output"

createfile bomfile.c "\xef\xbb\xbfa;
"

src2srcml bomfile.c

check 3<<< "$foutput"

# NOTE: This is broken in libsrcml
src2srcml --language 'C' < bomfile.c

check 3<<< "$output"
