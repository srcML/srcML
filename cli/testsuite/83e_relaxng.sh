#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT
INPUT
relaxng_error = STDOUTsrcml2src: Start tag expected '<' not found in '-'
	STDOUT

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


if sys.platform != 'cygwin' :
	checkError([srcml2src --relaxng + '=schema.rng' "" relaxng_error)
	srcml2src --relaxng + '=schema.rng

	check 4<<< "2"

srcml2src --relaxng + '=schema.rng' srcml srcml)
echo -n "" | srcml2src --relaxng + '=schema.rng' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --relaxng + '=schema.rng' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --relaxng + '=schema.rng' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


# TODO really make sure this is ok to return ok when no schema supplied.
srcml2src --relaxng srcml

check 4<<< "0"
if sys.platform != 'cygwin' :
	srcml2src --relaxng + '=' srcml

	check 4<<< "0"

# relaxng apply root

if sys.platform != 'cygwin' :
	checkError([srcml2src --apply-root --relaxng + '=schema.rng' "" relaxng_error)
	srcml2src --apply-root --relaxng + '=schema.rng

	check 4<<< "2"

srcml2src --apply-root --relaxng + '=schema.rng' srcml srcml)
echo -n "" | srcml2src --apply-root --relaxng + '=schema.rng' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --apply-root --relaxng + '=schema.rng' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --apply-root --relaxng + '=schema.rng' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


# TODO really make sure this is ok to return ok when no schema supplied.
srcml2src --apply-root --relaxng srcml

check 4<<< "0"
if sys.platform != 'cygwin' :
	srcml2src --apply-root --relaxng + '=' srcml

	check 4<<< "0"
