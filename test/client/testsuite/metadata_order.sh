#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# Test order of metadata option order

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="sub" filename="a.cpp" version="1.0"/>
	STDOUT

xmlcheck "$srcml"
createfile sub/a.cpp.xml "$srcml"

options=( --show-language --show-url --show-filename --show-src-version --show-encoding )

size="${#options[@]}"

define values <<- 'STDOUT'
	encoding="UTF-8"
	url="sub"
	filename="a.cpp"
	language="C++"
	version="1.0"
	STDOUT

index=-1

# Re-order the options as argument flags to srcml
new_arg() {
	index=$[$index+1]

	# Assign argument based on index value into options
	val="${options[(($index % $size))]} ${options[(($[$index+1] % $size))]} ${options[(($[$index+2] % $size))]} ${options[(($[$index+3] % $size))]} ${options[(($[$index+4] % $size))]}"

}

new_arg

srcml $val sub/a.cpp.xml
check "$values"

srcml $val < sub/a.cpp.xml
check "$values"

new_arg

srcml $val sub/a.cpp.xml
check "$values"

srcml $val < sub/a.cpp.xml
check "$values"

new_arg

srcml $val sub/a.cpp.xml
check "$values"

srcml $val < sub/a.cpp.xml
check "$values"

new_arg

srcml $val sub/a.cpp.xml
check "$values"

srcml $val < sub/a.cpp.xml
check "$values"

new_arg

srcml $val sub/a.cpp.xml
check "$values"

srcml $val < sub/a.cpp.xml
check "$values"
