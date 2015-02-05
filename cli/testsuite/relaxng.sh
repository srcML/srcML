#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT

define schema <<- 'STDOUT'
	<grammar xmlns="http://relaxng.org/ns/structure/1.0">
	  <start><ref name="anyElement"/></start>
	  <define name="anyElement">
	    <element><anyName/>
	      <zeroOrMore>
	      <choice>
	        <attribute><anyName/></attribute>
		    <text/>
			<ref name="anyElement"/>
		  </choice>
	      </zeroOrMore>
	    </element>
	  </define>
	</grammar>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# apply schema
createfile schema.rng "$schema"

srcml2src --relaxng=schema.rng sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "relaxng : schema.rng"

srcml2src --relaxng=schema.rng < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "relaxng : schema.rng"

srcml2src --relaxng=schema.rng sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "relaxng : schema.rng"

srcml2src --relaxng=schema.rng -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "relaxng : schema.rng"

srcml2src --relaxng=schema.rng -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "relaxng : schema.rng"

# apply root and apply schema
srcml2src --apply-root --relaxng=schema.rng sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "relaxng : schema.rng"

srcml2src --apply-root --relaxng=schema.rng < sub/a.cpp.xml
check 3<<< "$srcml" 4<<< "relaxng : schema.rng"

srcml2src --apply-root --relaxng=schema.rng sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "relaxng : schema.rng"

srcml2src --apply-root --relaxng=schema.rng -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "relaxng : schema.rng"

srcml2src --apply-root --relaxng=schema.rng -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$srcml" 4<<< "relaxng : schema.rng"


## TODO really make sure this is ok to return ok when no schema supplied.
#srcml2src --relaxng srcml
#
#check 4<<< "0"
#if sys.platform != 'cygwin' :
#	srcml2src --relaxng + '=' srcml
#
#	check 4<<< "0"
#

## TODO really make sure this is ok to return ok when no schema supplied.
#srcml2src --apply-root --relaxng srcml
#
#check 4<<< "0"
#if sys.platform != 'cygwin' :
#	srcml2src --apply-root --relaxng + '=' srcml
#
#	check 4<<< "0"
