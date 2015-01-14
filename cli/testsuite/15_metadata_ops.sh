#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get_attribute_all
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="bar" filename="foo" version="1.2"/>
	STDOUT

srcml --get-language <<< "$input"

check 3<<< "C++"

srcml --get-directory <<< "$input"

check 3<<< "bar"

srcml --get-filename <<< "$input"

check 3<<< "foo"

srcml --get-src-version <<< "$input"

check 3<<< "1.2"

srcml --get-encoding <<< "$input"

check 3<<< "UTF-8"

define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="" dir="" filename="" version=""/>
	STDIN

srcml --get-language <<< "$input"

check 3<<< ""

srcml --get-directory <<< "$input"

check 3<<< ""

srcml --get-filename <<< "$input"

check 3<<< ""

srcml --get-src-version <<< "$input"

check 3<<< ""

define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
	STDIN

# FIXME: Should be no output, not even a blank line
# srcml --get-language <<< "$input"

#check 3<<< ""

#srcml --get-directory <<< "$input"

#check 3<<< ""

#srcml --get-filename <<< "$input"

#check 3<<< ""

#srcml --get-src-version <<< "$input"

#check 3<<< ""

srcml2src --units <<< "$input"

check 3<<< "0"

