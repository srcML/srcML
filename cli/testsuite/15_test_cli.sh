#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get_attribute_all
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo" version="1.2"/>
	STDOUT

srcml2src --get-language <<< "$input"

check 3<<< "C++"

srcml2src --get-directory <<< "$input"

check 3<<< "bar"

srcml2src --get-filename <<< "$input"

check 3<<< "foo"

srcml2src --get-src-version <<< "$input"

check 3<<< "1.2"

srcml2src --get-encoding <<< "$input"

check 3<<< "UTF-8"

define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="" dir="" filename="" version=""/>
	STDIN

srcml2src --get-language <<< "$input"

check 3<<< ""

srcml2src --get-directory <<< "$input"

check 3<<< ""

srcml2src --get-filename <<< "$input"

check 3<<< ""

srcml2src --get-src-version <<< "$input"

check 3<<< ""

define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
	STDIN

srcml2src --get-language <<< "$input"

check 3<<< ""

srcml2src --get-directory <<< "$input"

check 3<<< ""

srcml2src --get-filename <<< "$input"

check 3<<< ""

srcml2src --get-src-version <<< "$input"

check 3<<< ""

srcml2src --units <<< "$input"

check 3<<< "0"

