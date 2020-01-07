#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh
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
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	break;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>

	</unit>
	STDOUT

xmlcheck "$output"

srcml -l C++ <<< "$input"
check "$output"

#srcml -l C++ --cpp-text-if0 <<< "$input"
#check "$output"

define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" options="CPP_MARKUP_IF0">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	<break>break;</break>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>

	</unit>
	STDOUT

xmlcheck "$output"

srcml -l C++ --cpp-markup-if0 <<< "$input"
check "$output2"
