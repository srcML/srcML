#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test --cpp-markup-if0
define input <<- 'INPUT'

	#if 0
	break;
	#endif
	INPUT

define fnomarkupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="50621da2dbdf215137525141dcf5022b780d28ba">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	break;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="50621da2dbdf215137525141dcf5022b780d28ba">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	break;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	</unit>
	STDOUT

define fmarkupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" options="CPP_MARKUP_IF0">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="50621da2dbdf215137525141dcf5022b780d28ba">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	<break>break;</break>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/b.cpp" hash="50621da2dbdf215137525141dcf5022b780d28ba">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	<break>break;</break>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	</unit>
	STDOUT

xmlcheck "$fnomarkupif0"
xmlcheck "$fmarkupif0"

createfile sub/a.cpp "$input"
createfile sub/b.cpp "$input"

# use default, where if 0 sections are not marked up
srcml sub/a.cpp sub/b.cpp
check "$fnomarkupif0"

srcml sub/a.cpp sub/b.cpp --archive
check "$fnomarkupif0"

srcml sub/a.cpp sub/b.cpp
check "$fnomarkupif0"

srcml sub/a.cpp sub/b.cpp
check "$fnomarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml
check sub/b.xml "$fnomarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml
check sub/b.xml "$fnomarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml
check sub/b.xml "$fnomarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml
check sub/b.xml "$fnomarkupif0"

# markup if0 regions
srcml sub/a.cpp sub/b.cpp --cpp-markup-if0
check "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp --cpp-markup-if0
check "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp --cpp-markup-if0
check "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp --cpp-markup-if0
check "$fmarkupif0"

srcml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check "$fmarkupif0"

srcml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check "$fmarkupif0"

srcml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check "$fmarkupif0"

srcml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml "$fmarkupif0"

srcml --cpp-markup-if0 sub/a.cpp sub/b.cpp -o sub/b.xml
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml sub/a.cpp sub/b.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml sub/a.cpp sub/b.cpp --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml sub/a.cpp sub/b.cpp --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml sub/a.cpp sub/b.cpp --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml sub/a.cpp sub/b.cpp --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml sub/a.cpp sub/b.cpp --cpp-markup-if0
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check sub/b.xml "$fmarkupif0"

