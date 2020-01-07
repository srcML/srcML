#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# get metadata combinations
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="sub" filename="a.cpp" version="1.2"/>
	STDOUT

define output <<- 'STDOUT'
	url="sub"
	filename="a.cpp"
	language="C++"
	STDOUT

xmlcheck "$input"
createfile sub/a.cpp.xml "$input"

# language, url, filename
srcml --show-language --show-url --show-filename sub/a.cpp.xml
check "$output"

srcml --show-language --show-url --show-filename < sub/a.cpp.xml
check "$output"

# url, language, filename
srcml --show-url --show-language --show-filename sub/a.cpp.xml
check "$output"

srcml --show-url --show-language --show-filename < sub/a.cpp.xml
check "$output"

# filename, url, language
srcml --show-filename --show-url --show-language sub/a.cpp.xml
check "$output"

srcml --show-filename --show-url --show-language < sub/a.cpp.xml
check "$output"

define output <<- 'STDOUT'
	encoding="UTF-8"
	url="sub"
	filename="a.cpp"
	language="C++"
	STDOUT

# language, url, filename, encoding
srcml --show-language --show-url --show-filename --show-encoding sub/a.cpp.xml
check "$output"

srcml --show-language --show-url --show-filename --show-encoding < sub/a.cpp.xml
check "$output"

# encoding, url, language, filename
srcml --show-encoding --show-url --show-language --show-filename sub/a.cpp.xml
check "$output"

srcml --show-encoding --show-url --show-language --show-filename < sub/a.cpp.xml
check "$output"

define output <<- 'STDOUT'
	encoding="UTF-8"
	url="sub"
	filename="a.cpp"
	language="C++"
	version="1.2"
	STDOUT

# filename, url, language, encoding, src version
srcml --show-filename --show-url --show-language --show-encoding --show-src-version sub/a.cpp.xml
check "$output"

srcml --show-filename --show-url --show-language --show-encoding --show-src-version < sub/a.cpp.xml
check "$output"

# version, language, encoding, filename, url
srcml --show-src-version --show-language --show-filename --show-encoding --show-url sub/a.cpp.xml
check "$output"

srcml --show-src-version --show-language --show-filename --show-encoding --show-url < sub/a.cpp.xml
check "$output"
