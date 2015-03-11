#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# get metadata combinations
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" directory="sub" filename="a.cpp" version="1.2"/>
	STDOUT


define output <<- 'STDOUT'
	language="C++"
	filename="a.cpp"
	directory="sub"
	STDOUT

createfile sub/a.cpp.xml "$input"

# language, directory, filename
srcml --show-language --show-directory --show-filename sub/a.cpp.xml
check 3<<< "$output"

srcml --show-language --show-directory --show-filename < sub/a.cpp.xml
check 3<<< "$output"

# directory, language, filename
srcml --show-directory --show-language --show-filename sub/a.cpp.xml
check 3<<< "$output"

srcml --show-directory --show-language --show-filename < sub/a.cpp.xml
check 3<<< "$output"

# filename, directory, language
srcml --show-filename --show-directory --show-language sub/a.cpp.xml
check 3<<< "$output"

srcml --show-filename --show-directory --show-language < sub/a.cpp.xml
check 3<<< "$output"


define output <<- 'STDOUT'
	language="C++"
	filename="a.cpp"
	directory="sub"
	encoding="UTF-8"
	STDOUT

# language, directory, filename, encoding
srcml --show-language --show-directory --show-filename --show-encoding sub/a.cpp.xml
check 3<<< "$output"

srcml --show-language --show-directory --show-filename --show-encoding < sub/a.cpp.xml
check 3<<< "$output"

# encoding, directory, language, filename
srcml --show-encoding --show-directory --show-language --show-filename sub/a.cpp.xml
check 3<<< "$output"

srcml --show-encoding --show-directory --show-language --show-filename < sub/a.cpp.xml
check 3<<< "$output"


define output <<- 'STDOUT'
	language="C++"
	filename="a.cpp"
	directory="sub"
	version="1.2"
	encoding="UTF-8"
	STDOUT

# filename, directory, language, encoding, src version
srcml --show-filename --show-directory --show-language --show-encoding --show-src-version sub/a.cpp.xml
check 3<<< "$output"

srcml --show-filename --show-directory --show-language --show-encoding --show-src-version < sub/a.cpp.xml
check 3<<< "$output"

# version, language, encoding, filename, directory
srcml --show-src-version --show-language --show-filename --show-encoding --show-directory sub/a.cpp.xml
check 3<<< "$output"

srcml --show-src-version --show-language --show-filename --show-encoding --show-directory < sub/a.cpp.xml
check 3<<< "$output"


