#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# Test order of metadata option order

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="sub" filename="a.cpp" version="1.0"/>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# TODO: Add get-hash and get-timestamp
options=( --show-language --show-url --show-filename --show-src-version --show-encoding )

size="${#options[@]}"

define values <<- 'STDOUT'
	language="C++"
	filename="a.cpp"
	url="sub"
	version="1.0"
	encoding="UTF-8"
	STDOUT

index=-1

# Re-order the options as argument flags to srcml
new_arg() {
	index=$[$index+1]

	# Assign argument based on index value into options
	val="${options[(($index % $size))]} ${options[(($[$index+1] % $size))]} ${options[(($[$index+2] % $size))]} ${options[(($[$index+3] % $size))]} ${options[(($[$index+4] % $size))]}"
	message "$val"

}

# TODO: This would be cleaner, if only bash didn't output the new_arg history instead
# of the srcml input

#for (( i = 0; i < $size; i++ )); do
#	new_arg
#
#	srcml2src $val sub/a.cpp.xml
#	check 3<<< "$values"
#
#	srcml2src $val < sub/a.cpp.xml
#	check 3<<< "$values"
#done

new_arg

srcml2src $val sub/a.cpp.xml
check 3<<< "$values"

srcml2src $val < sub/a.cpp.xml
check 3<<< "$values"


new_arg

srcml2src $val sub/a.cpp.xml
check 3<<< "$values"

srcml2src $val < sub/a.cpp.xml
check 3<<< "$values"


new_arg

srcml2src $val sub/a.cpp.xml
check 3<<< "$values"

srcml2src $val < sub/a.cpp.xml
check 3<<< "$values"


new_arg

srcml2src $val sub/a.cpp.xml
check 3<<< "$values"

srcml2src $val < sub/a.cpp.xml
check 3<<< "$values"


new_arg

srcml2src $val sub/a.cpp.xml
check 3<<< "$values"

srcml2src $val < sub/a.cpp.xml
check 3<<< "$values"


