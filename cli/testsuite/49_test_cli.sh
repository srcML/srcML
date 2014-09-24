#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++"/>
	STDOUT

echo -n "" | src2srcml -l C++ --no-namespace-decl

check 3<<< "$output"

define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++" filename="sub/a.cpp"/>
	STDOUT


createfile sub/a.cpp ""

src2srcml --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$output2"

##

# cpp markup else

define cpp_src <<- 'STDOUT'
	#if A
	break;
	#else
	return;
	#endif
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"><cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

createfile sub/a.cpp "$cpp_src"

src2srcml sub/a.cpp

check 3<<<"$output2"

exit 0

src2srcml -l C++ -o sub/a.cpp.xml <<- "$cpp_src"

check sub/a.cpp.xml 3<<<"$output"


src2srcml sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<<"$output"

src2srcml --cpp-markup-else sub/a.cpp "" fcpp_marked_srcml)

if sys.platform != 'cygwin' :
	src2srcml -l C++ --cpp-markup-else -o sub/a.cpp.xml cpp_src
	validate(open(sub/a.cpp.xml 'r').read() cpp_marked_srcml)
src2srcml --cpp-markup-else sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fcpp_marked_srcml)



define output <<- 'STDOUT'

	cpp_textonly_<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
INPUT
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
return;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
	STDOUT

define output <<- 'STDOUT'
	cpp_textonly_<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	INPUT
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
return;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
	STDOUT

src2srcml --cpp-text-else sub/a.cpp "" fcpp_textonly_srcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --cpp-text-else -o sub/a.cpp.xml cpp_src
	validate(open(sub/a.cpp.xml 'r').read() cpp_textonly_srcml)
src2srcml --cpp-text-else sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fcpp_textonly_srcm)



##

# cpp markup if0

cpp_if0 = STDOUT
#if 0
break;
#endif
	STDOUT

define output <<- 'STDOUT'
	cpp_textonly_<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
INPUT
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
	STDOUT

define output <<- 'STDOUT'
	cpp_textonly_<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	INPUT
break;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
	STDOUT

f = open(sub/a.cpp 'w')
f.write(cpp_if0)
f.close()

src2srcml sub/a.cpp "" fcpp_textonly_srcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ -o sub/a.cpp.xml cpp_if0
	validate(open(sub/a.cpp.xml 'r').read() cpp_textonly_srcm)
src2srcml sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fcpp_textonly_srcml)



src2srcml --cpp-text-if0 sub/a.cpp "" fcpp_textonly_srcml)

if sys.platform != 'cygwin' :
	src2srcml -l C++ --cpp-text-if0 -o sub/a.cpp.xml cpp_if0
	validate(open(sub/a.cpp.xml 'r').read() cpp_textonly_srcml)
src2srcml --cpp-text-if0 sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fcpp_textonly_srcml)



define output <<- 'STDOUT'

	cpp_marked_<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
INPUT
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
	STDOUT

define output <<- 'STDOUT'
	cpp_marked_<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	INPUT
<break>break;</break>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
	STDOUT

src2srcml --cpp-markup-if0 sub/a.cpp "" fcpp_marked_srcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --cpp-markup-if0 -o sub/a.cpp.xml cpp_if0
	validate(open(sub/a.cpp.xml 'r').read() cpp_marked_srcml)
src2srcml --cpp-markup-if0 sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fcpp_marked_srcml)



#

# Test srcml2src options with files

define output <<- 'STDOUT'
	sxmlfile1 = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" version="1.2">
	INPUT
</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.sdml.info/srcML/src">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	</src:unit>
	STDOUT

createfile sub/a.cpp.xml $sxmlfile1

# check metadata options
srcml2src sub/a.cpp.xml --get-language

check 3<<< "C++"

srcml2src sub/a.cpp.xml --get-directory

check 3<<< "sub"

srcml2src sub/a.cpp.xml --get-directory

check 3<<< "sub"

srcml2src sub/a.cpp.xml --get-directory

check 3<<< "sub"

srcml2src sub/a.cpp.xml --get-filename

check 3<<< "a.cpp"

srcml2src sub/a.cpp.xml --get-filename

check 3<<< "a.cpp"

srcml2src sub/a.cpp.xml --get-filename

check 3<<< "a.cpp"

srcml2src sub/a.cpp.xml --get-src-version

check 3<<< "1.2"

srcml2src sub/a.cpp.xml --get-src-version

check 3<<< "1.2"

srcml2src sub/a.cpp.xml --get-src-version

check 3<<< "1.2"

srcml2src sub/a.cpp.xml --get-encoding

check 3<<< "UTF-8"

srcml2src --units sub/a.cpp.xml

check 3<<< "1"

createfile sub/a.cpp.xml $nestedfile1

srcml2src --units sub/a.cpp.xml

check 3<<< "1"

createfile sub/a.cpp.xml $nestedfile2

srcml2src --units sub/a.cpp.xml

check 3<<< "2"

f = open(sub/a.cpp.xml 'w')

f.write(<<< "$nestedfilesrc)"
f.close()

srcml2src --units sub/a.cpp.xml "2\n")

# check unit option


define sfile1 <<- 'STDOUT'
	a;
	STDOUT

define sfile2 <<- 'STDOUT'
	b;
	STDOUT

define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
	</unit>
	STDOUT

define sxmlfile2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml $nestedfile

srcml2src sub/a.cpp.xml -U "1"

check 3<<< "$sfile1"

srcml2src sub/a.cpp.xml --unit "1"

check 3<<< "$sfile1"

srcml2src sub/a.cpp.xml --unit="1"

check 3<<< "$sfile1"

srcml2src --unit "1" -o sub/a.cpp <<< "$nestedfile"

readfile sub/a.cpp input

check 3<<< "$sfile1"

srcml2sc --unit "1" sub/a.cpp.xml -o sub/a.cpp
#validate(open(sub/a.cpp 'r').read()

check 3<<< "$sfile1"

srcml2src sub/a.cpp.xml -U "2" "" <<< "b;"
srcml2src sub/a.cpp.xml --unit "2" "" <<< "b;"
srcml2src sub/a.cpp.xml --unit="2" "" <<< "b;"

srcml2src --unit "2" -o "sub/b.cpp" <<< "$nestedfile"

#	readfile input sub/b.cpp' 'r').read() <<< "b;"

srcml2src --unit "2" sub/a.cpp.xml -o sub/b.cpp

#readfile input sub/b.cpp' 'r').read() <<< "b;"


# check xml and unit optin

srcml2src --xml --unit "1" sub/a.cpp.xml

check 3<<< "$sxmlfile1"

srcml2src --xml --unit "1" -o sub/b.cpp.xml <<< "$nestedfile"

check sub/b.cpp.xml <<< "$sxmlfile1"

srcml2src --xml --unit "1" sub/a.cpp.xml -o sub/b.cpp.xml

check sub/b.cpp.xml <<< "$sxmlfile1"

srcml2src --xml --unit "2" sub/a.cpp.xml 

check 3<<< "$sxmlfile2"

srcml2src --xml --unit "2" -o sub/b.cpp.xml <<< "$nestedfile"

check sub/b.cpp.xml "$xsmlfile2"

srcml2src --xml --unit "2" sub/a.cpp.xml -o sub/b.cpp.xml

check sub/b.cpp.xml "$xsmlfile2"


# check metadata options with xml and unit

define nestedfileextra <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">


	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="emptysrc" mytag=foo filename="empty.java">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$nestedfileextra"

srcml2src --xml --unit "1" --language sub/a.cpp.xml

check 4<<< "C++\n"

srcml2src --xml --unit "1" --directory sub/a.cpp.xml

check 4<<< "sub\n"

srcml2src --xml --unit "1" --filename sub/a.cpp.xml

check 4<<< "a.cpp\n"

srcml2src --xml --unit "2" --language sub/a.cpp.xml

check 3<<< "Java\n"

srcml2src --xml --unit "2" --directory sub/a.cpp.xml

check 3<<< "emptysrc\n"

srcml2src --xml --unit "2" --filename sub/a.cpp.xml

check 3<<< "empty.java\n"

# prefix extraction
define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$sxmlfile1"

src2srcml sub/a.cpp -o sub/a.cpp.xml

srcml2src sub/a.cpp.xml -p "http://www.sdml.info/srcML/src"

check 3<<< ""

srcml2src sub/a.cpp.xml --prefix "http://www.sdml.info/srcML/src"

check 3<<< ""

srcml2src sub/a.cpp.xml --prefix="http://www.sdml.info/srcML/src"

check 3<<< ""

srcml2src sub/a.cpp.xml -p "http://www.sdml.info/srcML/cpp"

check 3<<< "cpp"

srcml2src sub/a.cpp.xml --prefix "http://www.sdml.info/srcML/cpp"

check 3<<< "cpp"

srcml2src sub/a.cpp.xml --prefix="http://www.sdml.info/srcML/cpp"

check 3<<< "cpp"

srcml2src sub/a.cpp.xml -p "http://www.sdml.info/srcML/literal"

check 3<<< "lit"

srcml2src sub/a.cpp.xml --prefix "http://www.sdml.info/srcML/literal"

check 3<<< "lit"

srcml2src sub/a.cpp.xml --prefix="http://www.sdml.info/srcML/literal"

check 3<<< "lit"

srcml2src sub/a.cpp.xml -p "http://www.cs.uakron.edu/~collard/foo"

srcml2src sub/a.cpp.xml --prefix "http://www.cs.uakron.edu/~collard/foo"

srcml2src sub/a.cpp.xml --prefix="http://www.cs.uakron.edu/~collard/foo"

##

# text encoding flag

define sfile1 <<< "a;\n"

define sxmlfile1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
	</unit>
	STDOUT

createfile sub/a.cpp "$sfile1"

srcml2src --get-src-encoding "ISO-8859-1" sxmlfile1

check 3<<< "$sfile1"

srcml2src sub/a.cpp.xml --get-src-encoding "ISO-8859-1"

check 3<<< "$sfile1"

srcml2src --get-src-encoding "ISO-8859-1" sub/a.cpp.xml

check 3<<< "$sfile1"

srcml2src --get-src-encoding "ISO-8859-1" -o sub/a.cpp sxmlfile1

check sub/a.cpp "$sfile1"

srcml2src --get-src-encoding "ISO-8859-1" sub/a.cpp.xml -o sub/a.cpp

check sub/a.cpp "$sfile1"

##
