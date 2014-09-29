#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

##
# srcml2src info and longinfo

define info <<- 'STDOUT'
	xmlns="http://www.sdml.info/srcML/src"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	encoding="UTF-8"
	language="C++"
	directory="sub"
	filename="a.cpp"
	STDOUT

define longinfo <<- 'STDOUT'
	xmlns="http://www.sdml.info/srcML/src"
	xmlns:cpp="http://www.sdml.info/srcML/cpp"
	encoding="UTF-8"
	language="C++"
	directory="sub"
	filename="a.cpp"
	units="1"
	STDOUT

define longinfonested <<- 'STDOUT'
	xmlns="http://www.sdml.info/srcML/src"
	encoding="UTF-8"
	units="2"
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml << "$sxmlfile1"

srcml2src -i <<< "$sxmlfile"

check 3<<< "$info"

srcml2src --info <<< "$sxmlfile"

check 3<<< "$info"

srcml2src sub/a.cpp.xml -i

check 3<<< "$info"

srcml2src sub/a.cpp.xml --info

check 3<<< "$info"

srcml2src --longinfo <<< "$sxmlfile"

check 3<<< "$longinfo"

srcml2src sub/a.cpp.xml --longinfo

check 3<<< "$longinfo"

createfile sub/a.cpp.xml <<< "$nestedfile"

srcml2src -i --longinfo <<< "$nestedfile longinfonested)"
srcml2src -i --longinfo <<< "$nestedfile longinfonested)"
srcml2src -i --longinfo <<< "$nestedfile longinfonested)"
srcml2src sub/a.cpp.xml -i --longinfo longinfonested)
srcml2src sub/a.cpp.xml -i --longinfo longinfonested)
srcml2src sub/a.cpp.xml -i --longinfo longinfonested)



