#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

trap { cleanup; } EXIT

# test register language

define fxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="Java" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

src2srcml --register-ext cpp=Java sub/a.cpp

check 3<<< "$fxmlfile"

src2srcml --register-ext="cpp=Java" sub/a.cpp 

check 3<<< "$fxmlfile"

rmfile sub/a.cpp.xml

src2srcml --register-ext cpp=Java sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$fxmlfile"

#src2srcml -l C++ --register-ext 1)
#src2srcml -l C++ --register-ext "cpp=Jawa" )

define fsxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="Java" filename="sub/a.xml"/>
	STDOUT

createfile sub/a.xml ""

src2srcml --register-ext xml=Java sub/a.xml

check 3<<< "$fsxmlfile"

src2srcml --register-ext="xml=Java" sub/a.xml

check 3<<< "$fsxmlfile"

src2srcml --register-ext xml=Java sub/a.xml -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$fsxmlfile"
