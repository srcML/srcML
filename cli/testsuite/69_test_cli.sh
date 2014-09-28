#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# Test order of metadata option order

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" version="1.0"/>
	INPUT

options = [-l -d -f -s -x
	   -l -d -f -s -x

values = ['language="C++"\n' 'directory="sub"\n' 'filename="a.cpp"\n' 'src-version="1.0"\n' 'encoding="UTF-8"\n'
	  'language="C++"\n' 'directory="sub"\n' 'filename="a.cpp"\n' 'src-version="1.0"\n' 'encoding="UTF-8"\n'

index = 0
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

index += 1
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

index += 1
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

index += 1
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

index += 1
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

