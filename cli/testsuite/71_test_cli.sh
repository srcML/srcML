#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test expression_mode

define sxmlfile <<-'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr><name>a</name></expr>
	</unit>
	STDOUT

define fsxmlfile <<- 'STDOUT'
	<?xml vesion="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	<expr><name>a</name></expr>
	</unit>
	STDOUT

echo -n "a" >> sub/a.cpp

src2srcml -e <<< "a"

check 3<<< "$sxmlfile"

src2srcml --expression <<< "a"

check 3<<< "$sxmlfile

src2srcml sub/a.cpp -e

check 3<<< "$fsxmlfile"

src2srcml sub/a.cpp --expression

check 3<<< "$fsxmlfile"


#src2srcml -l C++ --expression -o sub/a.cpp.xml sfile
#validate(open(sub/a.cpp.xml 'r').read() sxmlfile)
#src2srcml --expression sub/a.cpp -o sub/a.cpp.xml

#validate(open(sub/a.cpp.xml 'r').read() fsxmlfile)



##

# Test Query and Transformation Options

# xpath

