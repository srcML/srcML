#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define sfile1 <<< "a;"

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

createfile sub/a.cpp "$sfile1"
createfile sub/a.cpp.xml "$foutput"

src2srcml sub/a.cpp --src-encoding "ISO-8859-1"
check 3<<< "$foutput"

src2srcml sub/a.cpp --src-encoding="ISO-8859-1"
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding="ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml sub/a.cpp.xml --src-encoding "ISO-8859-1"
check 3<<< "$sfile1"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp.xml
check 3<<< "$sfile1"

src2srcml --src-encoding "ISO-8859-1" -o sub/a.cpp sub/a.cpp.xml
check sub/a.cpp 3<<< "$sfile1"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp.xml -o sub/a.cpp
check sub/a.cpp 3<<< "$sfile1"

src2srcml -l C++ --src-encoding "ISO-8859-1" -o sub/a.cpp.xml sub/a.cpp
check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml 3<<< "$foutput"


# other supported encodings
src2srcml --src-encoding "UTF-8" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "UTF8" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "UTF-16" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "UTF16" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-10646-UCS-2" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "UCS-2" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "UCS2" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-10646-UCS-4" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "UCS-4" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "UCS4" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-LATIN-1" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO LATIN 1" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-2" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-LATIN-2" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO LATIN 2" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-3" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-4" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-5" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-6" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-7" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-8" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-8859-9" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "ISO-2022-JP" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "SHIFT_JIS" sub/a.cpp
check 3<<< "$foutput"

src2srcml --src-encoding "EUC-JP" sub/a.cpp
check 3<<< "$foutput"


# from one encoding to another
define special_characters_utf8_actual <<- 'STDOUT'
	typedef void ‼; // Also known as \u203C
	class ooɟ {
    	operator ‼() {}
	};
	STDOUT

define chinese_characters_utf8_actual <<- 'STDOUT'
	//源代码转换
	STDOUT

createfile sub/special_characters_utf8.cpp "$special_characters_utf8_actual"
createfile sub/chinese_characters_utf8.cpp "$chinese_characters_utf8_actual"

# create utf16 versions of the files
iconv -f UTF-8 -t UTF-16 sub/special_characters_utf8.cpp > sub/special_characters_utf16.cpp
iconv -f UTF-8 -t UTF-16 sub/chinese_characters_utf8.cpp > sub/chinese_characters_utf16.cpp

special_characters_utf16_actual=$(cat sub/special_characters_utf16.cpp)
chinese_characters_utf16_actual=$(cat sub/chinese_characters_utf16.cpp)

# create input srcML files from UTF16 encoding. use srcML files to output UTF16 files
src2srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp -o sub/special_characters_utf16.xml
src2srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp -o sub/chinese_characters_utf16.xml

srcml2src --src-encoding "UTF-16" sub/special_characters_utf16.xml -o sub/special_characters_utf16-srcml.cpp
check sub/special_characters_utf16-srcml.cpp 3<<< "$special_characters_utf16_actual"

srcml2src --src-encoding "UTF-16" sub/special_characters_utf16.xml
check 3<<< sub/special_characters_utf16_actual.cpp

srcml2src --src-encoding "UTF-16" sub/chinese_characters_utf16.xml -o sub/chinese_characters_utf16-srcml.cpp
check sub/chinese_characters_utf16-srcml.cpp 3<<< "$chinese_characters_utf16_actual"

srcml2src --src-encoding "UTF-16" sub/chinese_characters_utf16.xml
check 3<<< sub/chinese_characters_utf16_actual.cpp

# input UTF8 files and output UTF16 files
srcml --src-encoding "UTF-8" sub/special_characters_utf8.cpp --output-src -o sub/special_characters_utf16-srcml.cpp --src-encoding "UTF-16"
check sub/special_characters_utf16-srcml.cpp 3<<< "$special_characters_utf16_actual"

srcml --src-encoding "UTF-8" sub/chinese_characters_utf8.cpp --output-src -o sub/chinese_characters_utf16-srcml.cpp --src-encoding "UTF-16"
check sub/chinese_characters_utf16-srcml.cpp 3<<< "$chinese_characters_utf16_actual"

# input UTF16 files and output UTF8 files
srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp --output-src -o sub/special_characters_utf8-srcml.cpp --src-encoding "UTF-8"
check sub/special_characters_utf8-srcml.cpp 3<<< "$special_characters_utf8_actual"

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp --output-src -o sub/chinese_characters_utf8-srcml.cpp --src-encoding "UTF-8"
check sub/chinese_characters_utf8-srcml.cpp 3<<< "$chinese_characters_utf8_actual"

# input UTF16 files and output UTF16 files
srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp --output-src -o sub/special_characters_utf8-srcml.cpp --src-encoding "UTF-16"
check sub/special_characters_utf16-srcml.cpp 3<<< "$special_characters_utf16_actual"

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp --output-src -o sub/chinese_characters_utf8-srcml.cpp --src-encoding "UTF-16"
check sub/chinese_characters_utf16-srcml.cpp 3<<< "$chinese_characters_utf16_actual"


