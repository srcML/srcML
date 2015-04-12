#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test cpp markup else
define cpp_src <<- 'STDIN'
	#if A
	break;
	#else
	return;
	#endif
	STDIN

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++"><cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

createfile sub/a.cpp "$cpp_src"

src2srcml sub/a.cpp
check 3<<< "$output2"

src2srcml -l C++ -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$output"

src2srcml sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$output2"

#src2srcml --cpp-markup-else sub/a.cpp
#check 3<<< "$output2"

#src2srcml -l C++ --cpp-markup-else -o sub/a.cpp.xml < sub/a.cpp
#check sub/a.cpp.xml 3<<< "$output"

#src2srcml --cpp-markup-else sub/a.cpp -o sub/a.cpp.xml
#check sub/a.cpp.xml 3<<< "$output2"

# test cpp text else
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++"><cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	return;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	return;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

src2srcml --cpp-nomarkup-else sub/a.cpp
check 3<<< "$foutput"

src2srcml -l C++ --cpp-nomarkup-else -o sub/a.cpp.xml < sub/a.cpp
check sub/a.cpp.xml 3<<< "$output"

src2srcml --cpp-nomarkup-else sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"

