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
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	<break>break;</break>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

define fmarkupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	<break>break;</break>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

createfile sub/a.cpp "$input"


# use default, where if 0 sections are not marked up
src2srcml -l C++ < sub/a.cpp
check 3<<< "$nomarkupif0"

src2srcml -l C++ -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$nomarkupif0"

src2srcml -o sub/b.xml -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$nomarkupif0"

src2srcml sub/a.cpp
check 3<<< "$fnomarkupif0"

src2srcml sub/a.cpp -o sub/b.xml
check sub/b.xml 3<<< "$fnomarkupif0"


# markup if0 regions
src2srcml -l C++ --cpp-markup-if0 < sub/a.cpp
check 3<<< "$markupif0"

src2srcml --cpp-markup-if0 -l C++ < sub/a.cpp
check 3<<< "$markupif0"

src2srcml -l C++ --cpp-markup-if0 -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -l C++ -o sub/b.xml --cpp-markup-if0 < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -o sub/b.xml -l C++ --cpp-markup-if0 < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --cpp-markup-if0 -l C++ -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml sub/a.cpp --cpp-markup-if0
check 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp
check 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml sub/a.cpp --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp
check sub/b.xml 3<<< "$fmarkupif0"

