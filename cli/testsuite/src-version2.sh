#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test src version
define rev1 <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION" version="REVISION"/>
	STDIN

define rev2 <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="REVISION" version="0.9.0"/>
	STDIN

createfile sub/a.cpp.xml "$rev1"

srcml --show-src-version sub/a.cpp.xml
check 3<<< "REVISION"

srcml --show-src-version < sub/a.cpp.xml
check 3<<< "REVISION"

createfile sub/a.cpp.xml "$rev2"

srcml --show-src-version sub/a.cpp.xml
check 3<<< "0.9.0"

srcml --show-src-version < sub/a.cpp.xml
check 3<<< "0.9.0"
