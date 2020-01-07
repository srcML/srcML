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

define nomarkupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	break;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

define fnomarkupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	break;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

define markupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" options="CPP_MARKUP_IF0">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	<break>break;</break>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

define fmarkupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" options="CPP_MARKUP_IF0">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	<break>break;</break>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

xmlcheck "$nomarkupif0"
xmlcheck "$fnomarkupif0"
xmlcheck "$markupif0"
xmlcheck "$fmarkupif0"

createfile sub/a.cpp "$input"

# use default, where if 0 sections are not marked up
srcml -l C++ < sub/a.cpp
check "$nomarkupif0"

srcml -l C++ -o sub/b.xml < sub/a.cpp
check sub/b.xml "$nomarkupif0"

srcml -o sub/b.xml -l C++ < sub/a.cpp
check sub/b.xml "$nomarkupif0"

srcml sub/a.cpp
check "$fnomarkupif0"

srcml sub/a.cpp -o sub/b.xml
check sub/b.xml "$fnomarkupif0"

# markup if0 regions
srcml -l C++ --cpp-markup-if0 < sub/a.cpp
check "$markupif0"

srcml --cpp-markup-if0 -l C++ < sub/a.cpp
check "$markupif0"

srcml -l C++ --cpp-markup-if0 -o sub/b.xml < sub/a.cpp
check sub/b.xml "$markupif0"

srcml -l C++ -o sub/b.xml --cpp-markup-if0 < sub/a.cpp
check sub/b.xml "$markupif0"

srcml -o sub/b.xml -l C++ --cpp-markup-if0 < sub/a.cpp
check sub/b.xml "$markupif0"

srcml --cpp-markup-if0 -l C++ -o sub/b.xml < sub/a.cpp
check sub/b.xml "$markupif0"

srcml --cpp-markup-if0 -o sub/b.xml -l C++ < sub/a.cpp
check sub/b.xml "$markupif0"

srcml -o sub/b.xml --cpp-markup-if0 -l C++ < sub/a.cpp
check sub/b.xml "$markupif0"

srcml sub/a.cpp --cpp-markup-if0
check "$fmarkupif0"

srcml --cpp-markup-if0 sub/a.cpp
check "$fmarkupif0"

srcml sub/a.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml "$fmarkupif0"

srcml --cpp-markup-if0 sub/a.cpp -o sub/b.xml
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml sub/a.cpp --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp
check sub/b.xml "$fmarkupif0"

