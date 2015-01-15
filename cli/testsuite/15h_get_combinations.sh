#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# get metadata combinations
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="sub" filename="a.cpp" version="1.2"/>
	STDOUT


define output <<- 'STDOUT'
	language="C++"
	filename="a.cpp"
	directory="sub"
	STDOUT

createfile sub/a.cpp.xml "$input"

# language, directory, filename
srcml --get-language --get-directory --get-filename sub/a.cpp.xml
check 3<<< "$output"

srcml --get-language --get-directory --get-filename < sub/a.cpp.xml
check 3<<< "$output"

# directory, language, filename
srcml --get-directory --get-language --get-filename sub/a.cpp.xml
check 3<<< "$output"

srcml --get-directory --get-language --get-filename < sub/a.cpp.xml
check 3<<< "$output"

# filename, directory, language
srcml --get-filename --get-directory --get-language sub/a.cpp.xml
check 3<<< "$output"

srcml --get-filename --get-directory --get-language < sub/a.cpp.xml
check 3<<< "$output"


define output <<- 'STDOUT'
	language="C++"
	filename="a.cpp"
	directory="sub"
	encoding="UTF-8"
	STDOUT

# language, directory, filename, encoding
srcml --get-language --get-directory --get-filename --get-encoding sub/a.cpp.xml
check 3<<< "$output"

srcml --get-language --get-directory --get-filename --get-encoding < sub/a.cpp.xml
check 3<<< "$output"

# encoding, directory, language, filename
srcml --get-encoding --get-directory --get-language --get-filename sub/a.cpp.xml
check 3<<< "$output"

srcml --get-encoding --get-directory --get-language --get-filename < sub/a.cpp.xml
check 3<<< "$output"


define output <<- 'STDOUT'
	language="C++"
	filename="a.cpp"
	directory="sub"
	version="1.2"
	encoding="UTF-8"
	STDOUT

# filename, directory, language, encoding, src version
srcml --get-filename --get-directory --get-language --get-encoding --get-src-version sub/a.cpp.xml
check 3<<< "$output"

srcml --get-filename --get-directory --get-language --get-encoding --get-src-version < sub/a.cpp.xml
check 3<<< "$output"

# version, language, encoding, filename, directory
srcml --get-src-version --get-language --get-filename --get-encoding --get-directory sub/a.cpp.xml
check 3<<< "$output"

srcml --get-src-version --get-language --get-filename --get-encoding --get-directory < sub/a.cpp.xml
check 3<<< "$output"


