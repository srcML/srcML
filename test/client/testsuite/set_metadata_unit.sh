#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test metadata options with files
define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="sub" filename="a.cpp" version="1.2">
	</unit>
  STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.srcML.org/srcML/src">

	<src:unit revision="REVISION" language="C++" filename="sub/a.cpp">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	<src:unit revision="REVISION" language="C++" filename="sub/b.cpp">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	</src:unit>
  STDOUT

xmlcheck "$sxmlfile1"
xmlcheck "$nestedfile"
createfile sub/a.cpp.xml "$sxmlfile1"

# Deprecated warning message
define deprecated_warning <<- 'STDERR'
	srcml: use of option --units or -n is deprecated
STDERR

srcml sub/a.cpp.xml --show-language
check "C++"

srcml sub/a.cpp.xml --show-url
check "sub"

srcml sub/a.cpp.xml --show-filename
check "a.cpp"

srcml sub/a.cpp.xml --show-src-version
check "1.2"

srcml sub/a.cpp.xml --show-encoding
check "UTF-8"

srcml --units sub/a.cpp.xml
check "1" "$deprecated_warning"

createfile sub/nested.cpp.xml "$nestedfile"

srcml --units sub/nested.cpp.xml
check "2" "$deprecated_warning"
