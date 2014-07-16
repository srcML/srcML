#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# cpp markup else

define input <<- 'INPUT'
	
	#if A
	break;
	#else
	return;
	#endif
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>

	</unit>
	STDOUT

src2srcml -l C++ <<< "$input"

check 3<<< "$output"

src2srcml -l C++ --cpp-markup-else <<< "$input"

check 3<<< "$output"

define output <<- 'STDOUT'
    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	return;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

echo -n "" | src2srcml -l C++ --cpp-text-else --cpp_textonly_srcml

check 4<<< "15"

src2srcml -l C++ --cpp-markup-else --cpp-text-else foobar

check 4<<< "15"

src2srcml -l C++ --cpp-text-else --cpp-markup-else foobar

check 4<<< "15"

