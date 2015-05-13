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
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

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

createfile sub/a.cpp "$input"
createfile sub/b.cpp "$input"


# use default, where if 0 sections are not marked up
src2srcml sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fnomarkupif0"

src2srcml sub/a.cpp sub/b.cpp --in-order --archive
check 3<<< "$fnomarkupif0"

src2srcml sub/a.cpp --in-order sub/b.cpp
check 3<<< "$fnomarkupif0"

src2srcml --in-order sub/a.cpp sub/b.cpp
check 3<<< "$fnomarkupif0"

src2srcml sub/a.cpp sub/b.cpp -o sub/b.xml --in-order
check sub/b.xml 3<<< "$fnomarkupif0"

src2srcml sub/a.cpp --in-order sub/b.cpp -o sub/b.xml
check sub/b.xml 3<<< "$fnomarkupif0"

src2srcml --in-order sub/a.cpp sub/b.cpp -o sub/b.xml
check sub/b.xml 3<<< "$fnomarkupif0"

src2srcml sub/a.cpp sub/b.cpp --in-order -o sub/b.xml
check sub/b.xml 3<<< "$fnomarkupif0"


# markup if0 regions
src2srcml sub/a.cpp sub/b.cpp --cpp-markup-if0 --in-order
check 3<<< "$fmarkupif0"

src2srcml sub/a.cpp sub/b.cpp --in-order --cpp-markup-if0
check 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --in-order sub/b.cpp --cpp-markup-if0
check 3<<< "$fmarkupif0"

src2srcml --in-order sub/a.cpp sub/b.cpp --cpp-markup-if0
check 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp sub/b.cpp --in-order
check 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp --in-order sub/b.cpp
check 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 --in-order sub/a.cpp sub/b.cpp
check 3<<< "$fmarkupif0"

src2srcml --in-order --cpp-markup-if0 sub/a.cpp sub/b.cpp
check 3<<< "$fmarkupif0"

src2srcml sub/a.cpp sub/b.cpp --cpp-markup-if0 -o sub/b.xml --in-order
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp sub/b.cpp --cpp-markup-if0 --in-order -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp sub/b.cpp --in-order --cpp-markup-if0 -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --in-order sub/b.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --in-order sub/a.cpp sub/b.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp sub/b.cpp -o sub/b.xml --in-order
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp sub/b.cpp -o sub/b.xml --cpp-markup-if0 --in-order
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp sub/b.cpp -o sub/b.xml --in-order --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp sub/b.cpp --in-order -o sub/b.xml --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --in-order sub/b.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --in-order sub/a.cpp sub/b.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp sub/b.cpp --in-order
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp --in-order sub/b.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml --in-order sub/a.cpp sub/b.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 --in-order -o sub/b.xml sub/a.cpp sub/b.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --in-order --cpp-markup-if0 -o sub/b.xml sub/a.cpp sub/b.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml sub/a.cpp sub/b.cpp --cpp-markup-if0 --in-order
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml sub/a.cpp sub/b.cpp --in-order --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml sub/a.cpp --in-order sub/b.cpp --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --in-order sub/a.cpp sub/b.cpp --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --in-order -o sub/b.xml sub/a.cpp sub/b.cpp --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp sub/b.cpp --in-order
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp --in-order sub/b.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 --in-order sub/a.cpp sub/b.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --in-order --cpp-markup-if0 sub/a.cpp sub/b.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --in-order -o sub/b.xml --cpp-markup-if0 sub/a.cpp sub/b.cpp
check sub/b.xml 3<<< "$fmarkupif0"

