#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# cpp markup if0

define cpp_if0 <<- 'INPUT'
	#if 0
	break;
	#endif
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

src2srcml -l C++ cpp_if0 cpp_textonly_srcml)
src2srcml -l C++ --cpp-text-if0 cpp_if0 cpp_textonly_srcml)

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

src2srcml -l C++ --cpp-markup-if0 --cpp-text-if0 foobar

check 4<<< "15"

src2srcml -l C++ --cpp-text-if0 --cpp-markup-if0 foobar

check 4<<< "15"

