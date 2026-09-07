#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file xmlns_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test
defineXML output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
STDOUT

defineXML outputnocpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
STDOUT

defineXML foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
STDOUT

defineXML foutputnocpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
STDOUT

createfile sub/a.cpp "#define"

# separate
srcml --xmlns="http://www.srcML.org/srcML/src" sub/a.cpp
check "$foutputnocpp"

# the URI as a separate argument, instead of after an '='
defineXML fbaroutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:bar="http://www.foo.com" revision="REVISION" language="C++" filename="sub/a.cpp"><cpp:define>#<cpp:directive>define</cpp:directive></cpp:define></unit>
STDOUT

srcml --xmlns:bar="http://www.foo.com" sub/a.cpp
check "$fbaroutput"

srcml --xmlns "bar=http://www.foo.com" sub/a.cpp
check "$fbaroutput"

echo -n "#define" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src"
check "$outputnocpp"

echo -n "#define" | srcml -l C++ --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check "$output"

echo -n "#define" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" -o sub/a.cpp.xml
check sub/a.cpp.xml "$outputnocpp"

srcml --xmlns="http://www.srcML.org/srcML/src" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutputnocpp"

srcml --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check "$foutput"

echo -n "#define" | srcml -l C++ --xmlns:cpp="http://www.srcML.org/srcML/cpp" -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

srcml --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutput"

# multiple
echo -n "#define" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check "$output"

srcml --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check "$foutput"

echo -n "#define" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

srcml --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutput"
