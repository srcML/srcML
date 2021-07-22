#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define sfile1 <<< "a;"

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$foutput"
createfile sub/a.cpp "$sfile1"
createfile sub/a.cpp.xml "$foutput"

iconv -f UTF-8 -t ISO-8859-1 sub/a.cpp > sub/a_ISO-8859-1.cpp

srcml sub/a_ISO-8859-1.cpp --src-encoding "ISO-8859-1" --filename "sub/a.cpp"
check "$foutput"

srcml sub/a_ISO-8859-1.cpp --src-encoding="ISO-8859-1" --filename "sub/a.cpp"
check "$foutput"

srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

srcml --src-encoding="ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

srcml sub/a.cpp.xml --src-encoding "ISO-8859-1"
check "$sfile1"

srcml --src-encoding "ISO-8859-1" sub/a.cpp.xml
check "$sfile1"

srcml --src-encoding "ISO-8859-1" -o sub/a_ISO-8859-1.cpp sub/a.cpp.xml
check sub/a_ISO-8859-1.cpp "$sfile1"

srcml --src-encoding "ISO-8859-1" sub/a.cpp.xml -o sub/a_ISO-8859-1.cpp
check sub/a_ISO-8859-1.cpp "$sfile1"

srcml -l C++ --src-encoding "ISO-8859-1" -o sub/a.cpp.xml sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check sub/a.cpp.xml "$foutput"

srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp -o sub/a.cpp.xml --filename "sub/a.cpp"
check sub/a.cpp.xml "$foutput"

# other supported encodings
iconv -f UTF-8 -t UTF-8 sub/a.cpp > sub/a_UTF-8.cpp
srcml --src-encoding "UTF-8" sub/a_UTF-8.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t UTF-16 sub/a.cpp > sub/a_UTF-16.cpp
srcml --src-encoding "UTF-16" sub/a_UTF-16.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t UTF-16LE sub/a.cpp > sub/a_UTF-16LE.cpp
srcml --src-encoding "UTF-16LE" sub/a_UTF-16LE.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t UTF-16BE sub/a.cpp > sub/a_UTF-16BE.cpp
srcml --src-encoding "UTF-16BE" sub/a_UTF-16BE.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t UCS-2 sub/a.cpp > sub/a_UCS-2.cpp
srcml --src-encoding "UCS-2" sub/a_UCS-2.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t UCS-4 sub/a.cpp > sub/a_UCS-4.cpp
srcml --src-encoding "UCS-4" sub/a_UCS-4.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-1 sub/a.cpp > sub/a_ISO-8859-1.cpp
srcml --src-encoding "ISO-8859-1" sub/a_ISO-8859-1.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t LATIN1 sub/a.cpp > sub/a_LATIN1.cpp
srcml --src-encoding "LATIN1" sub/a_LATIN1.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-2 sub/a.cpp > sub/a_ISO-8859-2.cpp
srcml --src-encoding "ISO-8859-2" sub/a_ISO-8859-2.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t LATIN2 sub/a.cpp > sub/a_LATIN2.cpp
srcml --src-encoding "LATIN2" sub/a_LATIN2.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-3 sub/a.cpp > sub/a_ISO-8859-3.cpp
srcml --src-encoding "ISO-8859-3" sub/a_ISO-8859-3.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-4 sub/a.cpp > sub/a_ISO-8859-4.cpp
srcml --src-encoding "ISO-8859-4" sub/a_ISO-8859-4.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-5 sub/a.cpp > sub/a_ISO-8859-5.cpp
srcml --src-encoding "ISO-8859-5" sub/a_ISO-8859-5.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-6 sub/a.cpp > sub/a_ISO-8859-6.cpp
srcml --src-encoding "ISO-8859-6" sub/a_ISO-8859-6.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-7 sub/a.cpp > sub/a_ISO-8859-7.cpp
srcml --src-encoding "ISO-8859-7" sub/a_ISO-8859-7.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-8 sub/a.cpp > sub/a_ISO-8859-8.cpp
srcml --src-encoding "ISO-8859-8" sub/a_ISO-8859-8.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-8859-9 sub/a.cpp > sub/a_ISO-8859-9.cpp
srcml --src-encoding "ISO-8859-9" sub/a_ISO-8859-9.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t ISO-2022-JP sub/a.cpp > sub/a_ISO-2022-JP.cpp
srcml --src-encoding "ISO-2022-JP" sub/a_ISO-2022-JP.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t SHIFT_JIS sub/a.cpp > sub/a_SHIFT_JIS.cpp
srcml --src-encoding "SHIFT_JIS" sub/a_SHIFT_JIS.cpp --filename "sub/a.cpp"
check "$foutput"

iconv -f UTF-8 -t EUC-JP sub/a.cpp > sub/a_EUC-JP.cpp
srcml --src-encoding "EUC-JP" sub/a.cpp --filename "sub/a.cpp"
check "$foutput"

# # from one encoding to another

# Build output file using echo to get proper EOL
uncapture_output
touch sub/special_characters_utf8.cpp
echo -n "typedef void ‼; // Also known as \u203C"$EOL >> sub/special_characters_utf8.cpp
echo -n "class ooɟ {"$EOL >> sub/special_characters_utf8.cpp
echo -n "    operator ‼() {}"$EOL >> sub/special_characters_utf8.cpp
echo -n "};"$EOL >> sub/special_characters_utf8.cpp

echo -n "//源代码转换"$EOL > sub/chinese_characters_utf8.cpp
capture_output

# create utf16 versions of the files
uncapture_output
( printf "\xff\xfe" ; iconv -f utf-8 -t utf-16le sub/special_characters_utf8.cpp) > sub/special_characters_utf16.cpp
( printf "\xff\xfe" ; iconv -f utf-8 -t utf-16le sub/chinese_characters_utf8.cpp) > sub/chinese_characters_utf16.cpp

special_characters_utf16_actual=$(< sub/special_characters_utf16.cpp)
chinese_characters_utf16_actual=$(< sub/chinese_characters_utf16.cpp)
capture_output

# create input srcML files from UTF16 encoding. use srcML files to output UTF16 files
uncapture_output
srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp -o sub/special_characters_utf16.xml
srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp -o sub/chinese_characters_utf16.xml
capture_output

srcml --src-encoding "UTF-16" sub/special_characters_utf16.xml
check sub/special_characters_utf16.cpp

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.xml
check sub/chinese_characters_utf16.cpp

srcml --src-encoding "UTF-16" sub/special_characters_utf16.xml -o sub/special_characters_utf16-srcml.cpp
check_file sub/special_characters_utf16-srcml.cpp sub/special_characters_utf16.cpp

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.xml -o sub/chinese_characters_utf16-srcml.cpp
check_file sub/chinese_characters_utf16-srcml.cpp sub/chinese_characters_utf16.cpp

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.xml
check sub/chinese_characters_utf16.cpp

# input UTF8 files and output UTF16 files
srcml --src-encoding "UTF-8" sub/special_characters_utf8.cpp --output-src -o sub/special_characters_utf8-srcml.cpp
check_file sub/special_characters_utf8-srcml.cpp sub/special_characters_utf8.cpp

srcml --src-encoding "UTF-8" sub/chinese_characters_utf8.cpp --output-src -o sub/chinese_characters_utf8-srcml.cpp
check_file sub/chinese_characters_utf8-srcml.cpp sub/chinese_characters_utf8.cpp

# input UTF16 files and output UTF8 files
srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp --output-src -o sub/special_characters_utf16-srcml.cpp
check_file sub/special_characters_utf16-srcml.cpp sub/special_characters_utf16.cpp

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp --output-src -o sub/chinese_characters_utf16-srcml.cpp
check_file sub/chinese_characters_utf16-srcml.cpp sub/chinese_characters_utf16.cpp

# input UTF16 files and output UTF16 files
srcml --src-encoding "UTF-16" sub/special_characters_utf16.cpp --output-src -o sub/special_characters_utf16-srcml.cpp
check_file sub/special_characters_utf16-srcml.cpp sub/special_characters_utf16.cpp

srcml --src-encoding "UTF-16" sub/chinese_characters_utf16.cpp --output-src -o sub/chinese_characters_utf16-srcml.cpp
check_file sub/chinese_characters_utf16-srcml.cpp sub/chinese_characters_utf16.cpp

# Not supported on all platforms
if [[ "$OSTYPE" == 'msys' ]]; then
    exit 0
fi

srcml --src-encoding "UTF8" sub/a_UTF-8.cpp --filename "sub/a.cpp"
check "$foutput"

# Not supported on all platforms
if [[ "$OSTYPE" != "darwin"*  ]]; then
    exit 0
fi

uncapture_output
iconv -f UTF-8 -t ISO-10646-UCS-2 sub/a.cpp > sub/a_ISO-10646-UCS-2.cpp
capture_output
srcml --src-encoding "ISO-10646-UCS-2" sub/a_ISO-10646-UCS-2.cpp --filename "sub/a.cpp"
check "$foutput"

uncapture_output
iconv -f UTF-8 -t ISO-10646-UCS-4 sub/a.cpp > sub/a_ISO-10646-UCS-4.cpp
capture_output
srcml --src-encoding "ISO-10646-UCS-4" sub/a_ISO-10646-UCS-4.cpp --filename "sub/a.cpp"
check "$foutput"
