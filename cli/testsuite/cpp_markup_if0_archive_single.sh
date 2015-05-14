#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test --cpp-markup-if0
define input <<- 'INPUT'

	#if 0
	break;
	#endif
	INPUT

define nomarkupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="50621da2dbdf215137525141dcf5022b780d28ba">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	break;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	</unit>
	STDOUT

define fnomarkupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="50621da2dbdf215137525141dcf5022b780d28ba">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	break;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	</unit>
	STDOUT

define markupif0 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="50621da2dbdf215137525141dcf5022b780d28ba">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><literal type="number">0</literal></expr></cpp:if>
	<break>break;</break>
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

	</unit>
	STDOUT

createfile sub/a.cpp "$input"


# use default, where if 0 sections are not marked up
src2srcml -l C++ --archive < sub/a.cpp
check 3<<< "$nomarkupif0"

src2srcml --archive -l C++ < sub/a.cpp
check 3<<< "$nomarkupif0"

src2srcml -l C++ -o sub/b.xml --archive < sub/a.cpp
check sub/b.xml 3<<< "$nomarkupif0"

src2srcml -l C++ --archive -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$nomarkupif0"

src2srcml --archive -l C++ -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$nomarkupif0"

src2srcml -o sub/b.xml -l C++ --archive < sub/a.cpp
check sub/b.xml 3<<< "$nomarkupif0"

src2srcml -o sub/b.xml --archive -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$nomarkupif0"

src2srcml --archive -o sub/b.xml -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$nomarkupif0"

src2srcml sub/a.cpp --archive
check 3<<< "$fnomarkupif0"

src2srcml --archive sub/a.cpp
check 3<<< "$fnomarkupif0"

src2srcml sub/a.cpp -o sub/b.xml --archive
check sub/b.xml 3<<< "$fnomarkupif0"

src2srcml sub/a.cpp --archive -o sub/b.xml
check sub/b.xml 3<<< "$fnomarkupif0"

src2srcml --archive sub/a.cpp -o sub/b.xml
check sub/b.xml 3<<< "$fnomarkupif0"


# markup if0 regions
src2srcml -l C++ --cpp-markup-if0 --archive < sub/a.cpp
check 3<<< "$markupif0"

src2srcml -l C++ --archive --cpp-markup-if0 < sub/a.cpp
check 3<<< "$markupif0"

src2srcml --archive -l C++ --cpp-markup-if0 < sub/a.cpp
check 3<<< "$markupif0"

src2srcml --cpp-markup-if0 -l C++ --archive < sub/a.cpp
check 3<<< "$markupif0"

src2srcml --cpp-markup-if0 --archive -l C++ < sub/a.cpp
check 3<<< "$markupif0"

src2srcml --archive --cpp-markup-if0 -l C++ < sub/a.cpp
check 3<<< "$markupif0"

src2srcml -l C++ --cpp-markup-if0 -o sub/b.xml --archive < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -l C++ --cpp-markup-if0 --archive -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -l C++ --archive --cpp-markup-if0 -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --archive -l C++ --cpp-markup-if0 -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -l C++ -o sub/b.xml --cpp-markup-if0 --archive < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -l C++ -o sub/b.xml --archive --cpp-markup-if0 < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -l C++ --archive -o sub/b.xml --cpp-markup-if0 < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --archive -l C++ -o sub/b.xml --cpp-markup-if0 < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -o sub/b.xml -l C++ --cpp-markup-if0 --archive < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -o sub/b.xml -l C++ --archive --cpp-markup-if0 < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -o sub/b.xml --archive -l C++ --cpp-markup-if0 < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --archive -o sub/b.xml -l C++ --cpp-markup-if0 < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --cpp-markup-if0 -l C++ -o sub/b.xml --archive < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --cpp-markup-if0 -l C++ --archive -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --cpp-markup-if0 --archive -l C++ -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --archive --cpp-markup-if0 -l C++ -o sub/b.xml < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml -l C++ --archive < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml --archive -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --cpp-markup-if0 --archive -o sub/b.xml -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --archive --cpp-markup-if0 -o sub/b.xml -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 -l C++ --archive < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 --archive -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml -o sub/b.xml --archive --cpp-markup-if0 -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml --archive -o sub/b.xml --cpp-markup-if0 -l C++ < sub/a.cpp
check sub/b.xml 3<<< "$markupif0"

src2srcml sub/a.cpp --cpp-markup-if0 --archive
check 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --archive --cpp-markup-if0
check 3<<< "$fmarkupif0"

src2srcml --archive sub/a.cpp --cpp-markup-if0
check 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp --archive
check 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 --archive sub/a.cpp
check 3<<< "$fmarkupif0"

src2srcml --archive --cpp-markup-if0 sub/a.cpp
check 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --cpp-markup-if0 -o sub/b.xml --archive
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --cpp-markup-if0 --archive -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --archive --cpp-markup-if0 -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --archive sub/a.cpp --cpp-markup-if0 -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp -o sub/b.xml --archive
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp --archive -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 --archive sub/a.cpp -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --archive --cpp-markup-if0 sub/a.cpp -o sub/b.xml
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp -o sub/b.xml --cpp-markup-if0 --archive
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp -o sub/b.xml --archive --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml sub/a.cpp --archive -o sub/b.xml --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --archive sub/a.cpp -o sub/b.xml --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp --archive
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml --archive sub/a.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --cpp-markup-if0 --archive -o sub/b.xml sub/a.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --archive --cpp-markup-if0 -o sub/b.xml sub/a.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml sub/a.cpp --cpp-markup-if0 --archive
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml sub/a.cpp --archive --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --archive sub/a.cpp --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --archive -o sub/b.xml sub/a.cpp --cpp-markup-if0
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp --archive
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 --archive sub/a.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml -o sub/b.xml --archive --cpp-markup-if0 sub/a.cpp
check sub/b.xml 3<<< "$fmarkupif0"

src2srcml --archive -o sub/b.xml --cpp-markup-if0 sub/a.cpp
check sub/b.xml 3<<< "$fmarkupif0"

