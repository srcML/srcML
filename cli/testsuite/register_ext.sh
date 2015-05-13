#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test register language
define fxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

# src to srcml
src2srcml --register-ext cpp=Java sub/a.cpp
check 3<<< "$fxmlfile"

src2srcml --register-ext="cpp=Java" sub/a.cpp 
check 3<<< "$fxmlfile"

src2srcml --register-ext cpp=Java sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$fxmlfile"

define fsxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp"/>
	STDOUT

createfile sub/a.xml "$fsxmlfile"

src2srcml --register-ext xml=Java sub/a.xml
check_null

src2srcml --register-ext="xml=Java" sub/a.xml
check_null

