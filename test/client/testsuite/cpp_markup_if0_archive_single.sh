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

xmlcheck "$nomarkupif0"
xmlcheck "$fnomarkupif0"
xmlcheck "$markupif0"
xmlcheck "$fmarkupif0"

createfile sub/a.cpp "$input"


# use default, where if 0 sections are not marked up
src2srcml -l C++ --archive < sub/a.cpp
checkv2 "$nomarkupif0"

src2srcml --archive -l C++ < sub/a.cpp
checkv2 "$nomarkupif0"

src2srcml -l C++ -o sub/b.xml --archive < sub/a.cpp
checkv2 sub/b.xml "$nomarkupif0"

src2srcml -l C++ --archive -o sub/b.xml < sub/a.cpp
checkv2 sub/b.xml "$nomarkupif0"

src2srcml --archive -l C++ -o sub/b.xml < sub/a.cpp
checkv2 sub/b.xml "$nomarkupif0"

src2srcml -o sub/b.xml -l C++ --archive < sub/a.cpp
checkv2 sub/b.xml "$nomarkupif0"

src2srcml -o sub/b.xml --archive -l C++ < sub/a.cpp
checkv2 sub/b.xml "$nomarkupif0"

src2srcml --archive -o sub/b.xml -l C++ < sub/a.cpp
checkv2 sub/b.xml "$nomarkupif0"

src2srcml sub/a.cpp --archive
checkv2 "$fnomarkupif0"

src2srcml --archive sub/a.cpp
checkv2 "$fnomarkupif0"

src2srcml sub/a.cpp -o sub/b.xml --archive
checkv2 sub/b.xml "$fnomarkupif0"

src2srcml sub/a.cpp --archive -o sub/b.xml
checkv2 sub/b.xml "$fnomarkupif0"

src2srcml --archive sub/a.cpp -o sub/b.xml
checkv2 sub/b.xml "$fnomarkupif0"


# markup if0 regions
src2srcml -l C++ --cpp-markup-if0 --archive < sub/a.cpp
checkv2 "$markupif0"

src2srcml -l C++ --archive --cpp-markup-if0 < sub/a.cpp
checkv2 "$markupif0"

src2srcml --archive -l C++ --cpp-markup-if0 < sub/a.cpp
checkv2 "$markupif0"

src2srcml --cpp-markup-if0 -l C++ --archive < sub/a.cpp
checkv2 "$markupif0"

src2srcml --cpp-markup-if0 --archive -l C++ < sub/a.cpp
checkv2 "$markupif0"

src2srcml --archive --cpp-markup-if0 -l C++ < sub/a.cpp
checkv2 "$markupif0"

src2srcml -l C++ --cpp-markup-if0 -o sub/b.xml --archive < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -l C++ --cpp-markup-if0 --archive -o sub/b.xml < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -l C++ --archive --cpp-markup-if0 -o sub/b.xml < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --archive -l C++ --cpp-markup-if0 -o sub/b.xml < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -l C++ -o sub/b.xml --cpp-markup-if0 --archive < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -l C++ -o sub/b.xml --archive --cpp-markup-if0 < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -l C++ --archive -o sub/b.xml --cpp-markup-if0 < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --archive -l C++ -o sub/b.xml --cpp-markup-if0 < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -o sub/b.xml -l C++ --cpp-markup-if0 --archive < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -o sub/b.xml -l C++ --archive --cpp-markup-if0 < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -o sub/b.xml --archive -l C++ --cpp-markup-if0 < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --archive -o sub/b.xml -l C++ --cpp-markup-if0 < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --cpp-markup-if0 -l C++ -o sub/b.xml --archive < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --cpp-markup-if0 -l C++ --archive -o sub/b.xml < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --cpp-markup-if0 --archive -l C++ -o sub/b.xml < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --archive --cpp-markup-if0 -l C++ -o sub/b.xml < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml -l C++ --archive < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml --archive -l C++ < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --cpp-markup-if0 --archive -o sub/b.xml -l C++ < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --archive --cpp-markup-if0 -o sub/b.xml -l C++ < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 -l C++ --archive < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 --archive -l C++ < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml -o sub/b.xml --archive --cpp-markup-if0 -l C++ < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml --archive -o sub/b.xml --cpp-markup-if0 -l C++ < sub/a.cpp
checkv2 sub/b.xml "$markupif0"

src2srcml sub/a.cpp --cpp-markup-if0 --archive
checkv2 "$fmarkupif0"

src2srcml sub/a.cpp --archive --cpp-markup-if0
checkv2 "$fmarkupif0"

src2srcml --archive sub/a.cpp --cpp-markup-if0
checkv2 "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp --archive
checkv2 "$fmarkupif0"

src2srcml --cpp-markup-if0 --archive sub/a.cpp
checkv2 "$fmarkupif0"

src2srcml --archive --cpp-markup-if0 sub/a.cpp
checkv2 "$fmarkupif0"

src2srcml sub/a.cpp --cpp-markup-if0 -o sub/b.xml --archive
checkv2 sub/b.xml "$fmarkupif0"

src2srcml sub/a.cpp --cpp-markup-if0 --archive -o sub/b.xml
checkv2 sub/b.xml "$fmarkupif0"

src2srcml sub/a.cpp --archive --cpp-markup-if0 -o sub/b.xml
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --archive sub/a.cpp --cpp-markup-if0 -o sub/b.xml
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp -o sub/b.xml --archive
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --cpp-markup-if0 sub/a.cpp --archive -o sub/b.xml
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --cpp-markup-if0 --archive sub/a.cpp -o sub/b.xml
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --archive --cpp-markup-if0 sub/a.cpp -o sub/b.xml
checkv2 sub/b.xml "$fmarkupif0"

src2srcml sub/a.cpp -o sub/b.xml --cpp-markup-if0 --archive
checkv2 sub/b.xml "$fmarkupif0"

src2srcml sub/a.cpp -o sub/b.xml --archive --cpp-markup-if0
checkv2 sub/b.xml "$fmarkupif0"

src2srcml sub/a.cpp --archive -o sub/b.xml --cpp-markup-if0
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --archive sub/a.cpp -o sub/b.xml --cpp-markup-if0
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml sub/a.cpp --archive
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --cpp-markup-if0 -o sub/b.xml --archive sub/a.cpp
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --cpp-markup-if0 --archive -o sub/b.xml sub/a.cpp
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --archive --cpp-markup-if0 -o sub/b.xml sub/a.cpp
checkv2 sub/b.xml "$fmarkupif0"

src2srcml -o sub/b.xml sub/a.cpp --cpp-markup-if0 --archive
checkv2 sub/b.xml "$fmarkupif0"

src2srcml -o sub/b.xml sub/a.cpp --archive --cpp-markup-if0
checkv2 sub/b.xml "$fmarkupif0"

src2srcml -o sub/b.xml --archive sub/a.cpp --cpp-markup-if0
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --archive -o sub/b.xml sub/a.cpp --cpp-markup-if0
checkv2 sub/b.xml "$fmarkupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 sub/a.cpp --archive
checkv2 sub/b.xml "$fmarkupif0"

src2srcml -o sub/b.xml --cpp-markup-if0 --archive sub/a.cpp
checkv2 sub/b.xml "$fmarkupif0"

src2srcml -o sub/b.xml --archive --cpp-markup-if0 sub/a.cpp
checkv2 sub/b.xml "$fmarkupif0"

src2srcml --archive -o sub/b.xml --cpp-markup-if0 sub/a.cpp
checkv2 sub/b.xml "$fmarkupif0"

