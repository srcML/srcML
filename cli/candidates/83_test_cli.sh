#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
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

# position

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="8"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --position "" sxmlfile)
src2srcml --position sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --position -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --position sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile


# tabs

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="4"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename="sub/a.cpp" pos:tabs="4"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --tabs '4' "" sxmlfile)
src2srcml --tabs '4' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --tabs '4' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --tabs '4' sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile


src2srcml -l C++ --tabs 1)
src2srcml -l C++ --tabs 'a' 14)

# position and tabs

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="2"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename=sub/a.cpp pos:tabs="2"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --position --tabs '2' "" sxmlfile)
src2srcml --position --tabs '2' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --position --tabs '2' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --position --tabs '2' sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile


src2srcml -l C++ --position --tabs 1)
src2srcml -l C++ --position --tabs 'a' 14)

src2srcml -l C++ --tabs '2' --position "" sxmlfile)
src2srcml --tabs '2' --position sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --tabs '2' --position -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --tabs '2' --position sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --tabs --position 14)
src2srcml -l C++ --tabs 'a' --position 14)

# register language

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="Java" filename=sub/a.cpp/>
	STDOUT
INPUT
f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml --register-ext 'cpp=Java' sub/a.cpp "" fsxmlfile)
src2srcml --register-ext + '=cpp=Java' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml --register-ext 'cpp=Java' sub/a.cpp -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --register-ext 1)
src2srcml -l C++ --register-ext "cpp=Jawa" )

define output <<- 'STDOUT'
	fsxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="Java" filename="sub/a.xml"/>
	STDOUT
INPUT
if platform.system() != "Windows":
	os.system("touch su
b/a.xml")
else :
	os.system("copy emptysrc\\empty.cpp sub\\a.xml")

src2srcml --register-ext 'xml=Java' 'sub/a.xml' "" fsxmlfile)
src2srcml --register-ext + '=xml=Java' 'sub/a.xml' "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml --register-ext 'xml=Java' 'sub/a.xml' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --register-ext "xml=Jawa" 1)

##
# directory input

