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

xmlcheck "$foutput"
createfile sub/a.cpp "$sfile1"
createfile sub/a.cpp.xml "$foutput"

iconv -f UTF-8 -t ISO-8859-1 sub/a.cpp > sub/a_ISO-8859-1.cpp

src2srcml sub/a_ISO-8859-1.cpp --src-encoding "ISO-8859-1" --filename "sub/a.cpp"
check "$foutput"

src2srcml sub/a_ISO-8859-1.cpp --src-encoding="ISO-8859-1" --filename "sub/a.cpp"
check "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

src2srcml --src-encoding="ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

src2srcml sub/a.cpp.xml --src-encoding "ISO-8859-1"
check "$sfile1"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp.xml
check "$sfile1"

src2srcml --src-encoding "ISO-8859-1" -o sub/a_ISO-8859-1.cpp sub/a.cpp.xml
check sub/a_ISO-8859-1.cpp "$sfile1"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp.xml -o sub/a_ISO-8859-1.cpp
check sub/a_ISO-8859-1.cpp "$sfile1"

src2srcml -l C++ --src-encoding "ISO-8859-1" -o sub/a.cpp.xml sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check sub/a.cpp.xml "$foutput"

src2srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp -o sub/a.cpp.xml --filename "sub/a.cpp"
check sub/a.cpp.xml "$foutput"


# other supported encodings
iconv -f UTF-8 -t UTF-8 sub/a.cpp > sub/a_UTF-8.cpp
src2srcml --src-encoding "UTF-8" sub/a_UTF-8.cpp --filename "sub/a.cpp"
check "$foutput"

src2srcml --src-encoding "UTF8" sub/a_UTF-8.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t UTF-16LE sub/a.cpp > sub/a_UTF-16.cpp
src2srcml --src-encoding "UTF-16" sub/a_UTF-16.cpp --filename "sub/a.cpp"
check "$foutput"

src2srcml --src-encoding "UTF16" sub/a_UTF-16.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-10646-UCS-2 sub/a.cpp > sub/a_ISO-10646-UCS-2.cpp
src2srcml --src-encoding "ISO-10646-UCS-2" sub/a_ISO-10646-UCS-2.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t UCS-2 sub/a.cpp > sub/a_UCS-2.cpp
src2srcml --src-encoding "UCS-2" sub/a_UCS-2.cpp --filename "sub/a.cpp"
check "$foutput"

src2srcml --src-encoding "UCS2" sub/a_UCS-2.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-10646-UCS-4 sub/a.cpp > sub/a_ISO-10646-UCS-4.cpp
src2srcml --src-encoding "ISO-10646-UCS-4" sub/a_ISO-10646-UCS-4.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t UCS-4 sub/a.cpp > sub/a_USC-4.cpp
src2srcml --src-encoding "UCS-4" sub/a_USC-4.cpp --filename "sub/a.cpp"
check "$foutput"

src2srcml --src-encoding "UCS4" sub/a_USC-4.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-1 sub/a.cpp > sub/a_ISO-8859-1.cpp
src2srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

# No ISO-LATIN-1 in iconv
#iconv -f UTF-8 -t ISO-LATIN-1 sub/a.cpp > sub/a_ISO-LATIN-1.cpp
#src2srcml --src-encoding "ISO-LATIN-1" sub/a_ISO-LATIN-1.cpp --filename "sub/a.cpp"
#check "$foutput"

#src2srcml --src-encoding "ISO LATIN 1" sub/a_ISO-LATIN-1.cpp --filename "sub/a.cpp"
#check "$foutput"

iconv -f UTF-8 -t ISO-8859-2 sub/a.cpp > sub/a_ISO-8859-2.cpp
src2srcml --src-encoding "ISO-8859-2" sub/a_ISO-8859-2.cpp --filename "sub/a.cpp"
check "$foutput"

#iconv -f UTF-8 -t ISO-LATIN-2 sub/a.cpp > sub/a_ISO-LATIN-2.cpp
#src2srcml --src-encoding "ISO-LATIN-2" sub/a_ISO-LATIN-2.cpp --filename "sub/a.cpp"
#check "$foutput"

#src2srcml --src-encoding "ISO LATIN 2" sub/a_ISO-LATIN-2.cpp --filename "sub/a.cpp"
#check "$foutput"

iconv -f UTF-8 -t ISO-8859-3 sub/a.cpp > sub/a_ISO-8859-3.cpp
src2srcml --src-encoding "ISO-8859-3" sub/a_ISO-8859-3.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-4 sub/a.cpp > sub/a_ISO-8859-4.cpp
src2srcml --src-encoding "ISO-8859-4" sub/a_ISO-8859-4.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-5 sub/a.cpp > sub/a_ISO-8859-5.cpp
src2srcml --src-encoding "ISO-8859-5" sub/a_ISO-8859-5.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-6 sub/a.cpp > sub/a_ISO-8859-6.cpp
src2srcml --src-encoding "ISO-8859-6" sub/a_ISO-8859-6.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-7 sub/a.cpp > sub/a_ISO-8859-7.cpp
src2srcml --src-encoding "ISO-8859-7" sub/a_ISO-8859-7.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-8 sub/a.cpp > sub/a_ISO-8859-8.cpp
src2srcml --src-encoding "ISO-8859-8" sub/a_ISO-8859-8.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-9 sub/a.cpp > sub/a_ISO-8859-9.cpp
src2srcml --src-encoding "ISO-8859-9" sub/a_ISO-8859-9.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-2022-JP sub/a.cpp > sub/a_ISO-2022-JP.cpp
src2srcml --src-encoding "ISO-2022-JP" sub/a_ISO-2022-JP.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t SHIFT_JIS sub/a.cpp > sub/a_SHIFT_JIS.cpp
src2srcml --src-encoding "SHIFT_JIS" sub/a_SHIFT_JIS.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t EUC-JP sub/a.cpp > sub/a_EUC-JP.cpp
src2srcml --src-encoding "EUC-JP" sub/a.cpp --filename "sub/a.cpp"
check "$foutput"


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
iconv -f UTF-8 -t UTF-16LE sub/special_characters_utf8.cpp > sub/special_characters_utf16.cpp
iconv -f UTF-8 -t UTF-16LE sub/chinese_characters_utf8.cpp > sub/chinese_characters_utf16.cpp

special_characters_utf16_actual=$(cat sub/special_characters_utf16.cpp)
chinese_characters_utf16_actual=$(cat sub/chinese_characters_utf16.cpp)

# create input srcML files from UTF16 encoding. use srcML files to output UTF16 files
src2srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp -o sub/special_characters_utf16.xml
src2srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp -o sub/chinese_characters_utf16.xml

srcml2src --src-encoding "UTF-16" sub/special_characters_utf16.xml -o sub/special_characters_utf16-srcml.cpp
check sub/special_characters_utf16-srcml.cpp "$special_characters_utf16_actual"

srcml2src --src-encoding "UTF-16" sub/special_characters_utf16.xml
check sub/special_characters_utf16_actual.cpp

srcml2src --src-encoding "UTF-16" sub/chinese_characters_utf16.xml -o sub/chinese_characters_utf16-srcml.cpp
check sub/chinese_characters_utf16-srcml.cpp "$chinese_characters_utf16_actual"

srcml2src --src-encoding "UTF-16" sub/chinese_characters_utf16.xml
check sub/chinese_characters_utf16_actual.cpp

# input UTF8 files and output UTF16 files
srcml --src-encoding "UTF-8" sub/special_characters_utf8.cpp --output-src -o sub/special_characters_utf16-srcml.cpp --src-encoding "UTF-16"
check sub/special_characters_utf16-srcml.cpp "$special_characters_utf16_actual"

srcml --src-encoding "UTF-8" sub/chinese_characters_utf8.cpp --output-src -o sub/chinese_characters_utf16-srcml.cpp --src-encoding "UTF-16"
check sub/chinese_characters_utf16-srcml.cpp "$chinese_characters_utf16_actual"

# input UTF16 files and output UTF8 files
srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp --output-src -o sub/special_characters_utf8-srcml.cpp --src-encoding "UTF-8"
check sub/special_characters_utf8-srcml.cpp "$special_characters_utf8_actual"

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp --output-src -o sub/chinese_characters_utf8-srcml.cpp --src-encoding "UTF-8"
check sub/chinese_characters_utf8-srcml.cpp "$chinese_characters_utf8_actual"

# input UTF16 files and output UTF16 files
srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp --output-src -o sub/special_characters_utf8-srcml.cpp --src-encoding "UTF-16"
check sub/special_characters_utf16-srcml.cpp "$special_characters_utf16_actual"

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp --output-src -o sub/chinese_characters_utf8-srcml.cpp --src-encoding "UTF-16"
check sub/chinese_characters_utf16-srcml.cpp "$chinese_characters_utf16_actual"


