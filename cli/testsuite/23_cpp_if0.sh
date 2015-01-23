#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh
set +e
# test
##
# cpp markup if0

define input <<- 'INPUT'
	#if 0
	break;
	#endif
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
	break;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

src2srcml -l C++ <<< "$input"

check 3<<< "$output"

src2srcml -l C++ --cpp-text-if0 <<< "$input"

check 3<<< "$output"

define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
	<break>break;</break>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

src2srcml -l C++ --cpp-markup-if0 <<< "$input"

check 3<<< "$output2"

src2srcml -l C++ --cpp-markup-if0 --cpp-text-if0 foobar

check_exit 15

src2srcml -l C++ --cpp-text-if0 --cpp-markup-if0 foobar

check_exit 15

