#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test simple
echo -n "" | src2srcml -l C++

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

# test debug_namespace
define output <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
	OUTPUT

echo -n "" | src2srcml -l C++ -g

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --debug

check 3<<< "$output"

# test language_attribute_cpp
define output <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	OUTPUT

echo -n "" | src2srcml -l "C++"

check 3<<< "$output"

echo -n "" | src2srcml --language "C++"

check 3<<< "$output"

echo -n "" | src2srcml --language="C++"

check 3<<< "$output"

# test language_attribute_c
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
	STDOUT

echo -n "" | src2srcml -l "C"

check 3<<< "$output"

echo -n "" | src2srcml --language "C"

check 3<<< "$output"

echo -n "" | src2srcml --language="C"

check 3<<< "$output"

# test language_attribute_java
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="Java"/>
	STDOUT

echo -n "" | src2srcml -l Java

check 3<<< "$output"

echo -n "" | src2srcml --language Java

check 3<<< "$output"

echo -n "" | src2srcml --language=Java

check 3<<< "$output"

# test filename_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo"/>
	STDOUT

echo -n "" | src2srcml -l C++ -f foo

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --filename foo

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --filename="foo"

check 3<<< "$output"

# test filename_attribute_path
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="bar/foo"/>
	STDOUT

echo -n "" | src2srcml -l C++ -f bar/foo

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --filename bar/foo

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --filename=bar/foo

check 3<<< "$output"

# test filename_directory_attribute
echo -n "" | src2srcml -l C++ -f foo -d bar

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo"/>
	STDOUT

echo -n "" | src2srcml -l C++ -d bar -f foo

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo"/>
	STDOUT

# test directory_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar"/>
	STDOUT

echo -n "" | src2srcml -l C++ -d bar

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --directory bar

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --directory=bar

check 3<<< "$output"

# test src-version_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" version="1.0"/>
	STDOUT

echo -n "" | src2srcml -l C++ -s 1.0

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --src-version 1.0

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --src-version=1.0

check 3<<< "$output"

# test xml_encoding_attribute
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

echo -n "" | src2srcml -l C++ -x "ISO-8859-1"

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --encoding "ISO-8859-1"

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --encoding="ISO-8859-1"

check 3<<< "$output"

# test output_file
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

src2srcml -l C++ <<< "
a;"

check 3<<< "$output"

mkdir -p sub

define sfile1 <<- 'STDOUT'
	a;
	STDOUT

src2srcml -l C++ - -o sub/a.cpp.xml <<< $sfile1

check sub/a.cpp.xml 3<<< "$output"

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	STDOUT

src2srcml -l C++ <<< "
b;
"

check 3<<< "$output"

src2srcml -l C++ - -o sub/b.cpp.xml <<< "
b;
"

check sub/b.cpp.xml 3<<< "$output"

# test archive
define fileasrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc" language="C++" filename="sub/b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define nestedfilesrc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" language="C++" filename="sub/a.cpp">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	</src:unit>
	STDOUT

mkdir -p sub
echo -e "\na;" > sub/a.cpp
echo -e "\nb;" > sub/b.cpp

src2srcml sub/a.cpp sub/b.cpp -o - 

check 3<<< "$nestedfile"

src2srcml --archive sub/a.cpp -o -

check 3<<< "$fileasrcml"

define filelist <<- 'STDOUT'
	sub/a.cpp
	# fff
	sub/b.cpp
	STDOUT

createfile filelistab "$filelist"

#src2srcml --files-from filelistab

#check 3<< "$nestedfile"

message "A"

rm -f filelistab

# test srcml_extraction

define src <<< "a;"

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

srcml2src <<< "$output"

check 3<<< "$src"

echo "$output" > sub/a.cpp.xml

srcml2src sub/a.cpp.xml

check 3<<< "$src"

# test get_attribute_all
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo" version="1.2"/>
	STDOUT

srcml2src --get-language <<< "$input"

check 3<<< "C++"

srcml2src --get-directory <<< "$input"

check 3<<< "bar"

srcml2src --get-filename <<< "$input"

check 3<<< "foo"

srcml2src --get-version <<< "$input"

check 3<<< "1.2"

srcml2src --get-encoding <<< "$input"

check 3<<< "UTF-8"

define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="" dir="" filename="" version=""/>
	STDIN

srcml2src --get-language <<< "$input"

check 3<<< ""

srcml2src --get-directory <<< "$input"

check 3<<< ""

srcml2src --get-filename <<< "$input"

check 3<<< ""

srcml2src --get-version <<< "$input"

check 3<<< ""

define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
	STDIN

srcml2src --get-language <<< "$input"

check 3<<< ""

srcml2src --get-directory <<< "$input"

check 3<<< ""

srcml2src --get-filename <<< "$input"

check 3<<< ""

srcml2src --get-version <<< "$input"

check 3<<< ""

srcml2src --units <<< "$input"

check 3<<< "0"

# test
srcml2src --units <<< "$nestedfile"

check 4<<< "2"

srcml2src --units <<< "$nestedfilesrc"

check 4<<< "2"

srcml2src -U "1" <<< "$nestedfile"

check 3<<< "$sfile1"

srcml2src --unit "1" <<< "$nestedfile"

check 3<<< "$sfile1"

srcml2src --unit="1" <<< "$nestedfile"

check 3<<< "$sfile1"

srcml2src -U "2" <<< "$nestedfile"

check 3<<< "b;"

srcml2src --unit "2" <<< "$nestedfile"

check 3<<< "b;"

srcml2src --unit="2" <<< "$nestedfile"

check 3<<< "b;"

# test
define sxmlfile1 <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
	</unit>
	STDIN

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

srcml2src --xml --unit "1" - <<< "$nestedfile"

check 3<<< "$sxmlfile1"

srcml2src --xml --unit "1" <<< "$nestedfile"

check 3<<< "$sxmlfile1"

define sxmlfile2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	</unit>
	STDOUT

srcml2src --xml --unit "2" - <<< "$nestedfile"

check 3<<< "$sxmlfile2"

srcml2src --xml --unit "2" <<< "$nestedfile"

check 3<<< "$sxmlfile2"


# test
rm -f sub/a.cpp

#remove
# if platform.system() != "Windows" and sys.platform != 'cygwin' :
#	 checkNoOutput([srcml2sr --to-dir + '=.' sxmlfile1)

#	 validate(open(sub/a.cpp "r").read()

#	 check 3<<< "$sfile1"


#remove
rm -f sub/a.cpp sub/b.cpp;

# if platform.system() != "Windows" and sys.platform != 'cygwin' :
#	 checkNoOutput([srcml2sr --to-dir + '=.' <<< "$nestedfile)

#	 validate(open(sub/a.cpp "r").read()

#	 check 3<<< "$sfile1"

#	 validate(open("sub/b.cpp" "r").read() <<< "b;"

#os.system("rm -f sub/a.cpp sub/b.cpp")

#checkNoOutput([srcml2sr -a '.' <<< "$nestedfile)"

#validate(open(sub/a.cpp "r").read()

check 3<<< "$sfile1"

#validate(open("sub/b.cpp" "r").read() <<< "b;"

# srcml2src extract nested unit
define nestedfileextra <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="sub" mytag=foo filename="b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" dir="sub" filename="a.cpp" mytag=foo>
	</unit>
	STDOUT

srcml2src --xml --unit "1" - << $input

check 3<<< "$file1"

srcml2src --xml --unit "1" << $input

check 3<<< "$file1"

srcml2src --xml --unit "1" --get-filename << $input

check 3<<< "a.cpp"

srcml2src --xml --unit "2" --get-filename << $input

check 3<<- 'STDOUT'
	b.cpp
	STDOUT
	
srcml2src --xml --unit "1" --get-directory << $input

check 3<<< "sub"

srcml2src --xml --unit "2" --get-directory << $input

check 3<<< "sub"

srcml2src --xml --unit "1" --get-language << $input

check 3<<- 'STDOUT'
	C
	STDOUT
	
srcml2src --xml --unit "2" --get-language << $input

check 3<<- 'STDOUT'
	Java
	STDOUT
	
# test extract_archive_unit

define input <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:lc="http://www.sdml.info/srcML/linecol" language="C++">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="sub" mytag=foo filename="a.cpp">
	<expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="sub" mytag=foo filename="b.cpp">
	<expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" dir="sub" mytag=foo filename="c.cpp">
	<expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
	</unit>

	</unit>
	INPUT

define file1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:lc="http://www.sdml.info/srcML/linecol" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" dir="sub" filename="a.cpp" mytag=foo>
	</unit>
	STDOUT

define file2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:lc="http://www.sdml.info/srcML/linecol" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="sub" mytag=foo filename="b.cpp">
	</unit>
	STDOUT

define file3 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:lc="http://www.sdml.info/srcML/linecol" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" mytag=foo filename="c.cpp">
	</unit>
	STDOUT

srcml2src --xml --unit "1" - <<< "$input"

check 3<<< "$file1"

srcml2src --xml --unit "1" <<< "$input"

check 3<<< "$file1"

srcml2src --xml --unit "2" - <<< "$input"

check 3<<< "$file2"

srcml2src --xml --unit "2" <<< "$input"

check 3<<< "$file2"

srcml2src --xml --unit "3" - <<< "$input"

check 3<<< "$file3"

srcml2src --xml --unit "3" <<< "$input"

check 3<<< "$file3"

srcml2src --xml --unit "1" --get-filename <<< "$input"

check 3<<< "a.cpp"

srcml2src --xml --unit "2" --get-filename <<< "$input"

check 3<<< "b.cpp"

srcml2src --xml --unit "1" --get-directory <<< "$input"

check 3<<< "sub"

srcml2src --xml --unit "2" --get-directory <<< "$input"

check 3<<< "sub"

srcml2src --xml --unit "1" --get-language <<< "$input"

check 3<<< "C"

srcml2src --xml --unit "2" --get-language <<< "$input"

check 3<<< "Java"

# test invalid_input

# unknown file
src2srcml -l C++ foobar

#sd inputflename
#src2srcml sub/a.cpp" -o sub/a.cpp None) 2)
#wn option

src2srcml -l C++ --strip << "a;"

check 4<< "3"

# unknown encoding
src2srcml -l C++ --src-encoding="ISO" <<< "a;"

check 4<< "4"

src2srcml -l C++ --encoding="ISO" <<< "a;"

check 4<< "4"

# missing value
src2srcml --language bad_language "foobar"

check 4<< "6"

src2srcml --language

check 4<< "7"

src2srcml -l C++ --filename 8

check 4<< "8"

src2srcml -l C++ --directory 9

check 4<< "9"

src2srcml -l C++ --src-version 9

check 4<< "9"

# source encoding not given
src2srcml -l C++ --src-encoding

check 4<< "11"

# xml encoding not given
src2srcml -l C++ --encoding

check 4<< "12"

# test
##
# srcml2src error return

# invalid input filename
srcml2src foobar

check 4<<< "2"

# unknown option
srcml2src --strip foobar

check 4<<< "3"

# unknown encoding
srcml2src --src-encoding="ISO"

check 4<<< "4"

srcml2src --src-encoding

check 4<<< "1"
	
# source encoding not given

# unit option selected but no value
srcml2src --unit

check 4<<< "3"

# unit value too large

srcml2src --unit 3 <<< "$nestedfile"

check 4<<< "4"

srcml2src --unit 3 --xml <<< "$nestedfile"

check 4<<< "4"

srcml2src --unit 3 --filename <<< "$nestedfile"

check 4<<< "4"

srcml2src --unit 3 --directory <<< "$nestedfile"

check 4<<< "4"

srcml2src --unit 3 --src-version <<< "$nestedfile"

check 4<<< "4"

# invalid combinations
srcml2src --xml --src-encoding "UTF-8" "foobar"

check 4<<< "5"

# test
##
# cpp markup else

define cpp_src <<- 'INPUT'
	#if A
	break;
	#else
	return;
	#endif
	INPUT

define output <<- 'STDOUT'
    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

src2srcml -l C++ cpp_src <<< "$input"

check 3<<< "$cpp_marked_srcml"

src2srcml -l C++ --cpp-markup-else <<< "$input"

check 3<<< "$cpp_marked_srcml"

define output <<- 'STDOUT'
    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	return;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

echo -n "" | src2srcml -l C++ --cpp-text-else --cpp_textonly_srcml

check 4<<< "15"

src2srcml -l C++ --cpp-markup-else --cpp-text-else foobar

check 4<<< "15"

src2srcml -l C++ --cpp-text-else --cpp-markup-else foobar

check 4<<< "15"

# test
##
# cpp markup if0

define cpp_if0 <<- 'INPUT'
	#if 0
	break;
	#endif
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

src2srcml -l C++ cpp_if0 cpp_textonly_srcml)
src2srcml -l C++ --cpp-text-if0 cpp_if0 cpp_textonly_srcml)

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>
	STDOUT

src2srcml -l C++ --cpp-markup-if0 --cpp-text-if0 foobar

check 4<<< "15"

src2srcml -l C++ --cpp-text-if0 --cpp-markup-if0 foobar

check 4<<< "15"

# test
##
# xmlns options

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

# separate
echo -n "" | src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src"

check 3<<< "$output"

echo -n "" | src2srcml -l C++ --xmlns:cpp="http://www.sdml.info/srcML/cpp"

check 3<<< "$output"

# multiple

echo -n "" | src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp"

check 3<<< "$output"

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
	STDOUT

# separate
src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" 
echo -n "" | --xmlns:err="http://www.sdml.info/srcML/srcerr"


# multiple
echo -n "" | src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp"

echo -n "" | src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr"

echo -n "" | src2srcml -l C++ --debug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr"

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	STDOUT

##
# prefix extraction
define input <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
	STDIN

srcml2src -p "http://www.sdml.info/srcML/src" <<< "$input"

check 3<<< ""

srcml2src --prefix "http://www.sdml.info/srcML/src" <<< "$input"

check 3<<< ""

srcml2src --prefix="http://www.sdml.info/srcML/src" <<< "$input"

check 3<<< ""

srcml2src -p "http://www.sdml.info/srcML/cpp" <<< "$input"

check 3<<< "cpp"

srcml2src --prefix "http://www.sdml.info/srcML/cpp" <<< "$input"

check 3<<< "cpp"

srcml2src --prefix="http://www.sdml.info/srcML/cpp" <<< "$input"

check 3<<< "cpp"

srcml2src -p "http://www.sdml.info/srcML/literal" <<< "$input"

check 3<<< "$input"

srcml2src --prefix "http://www.sdml.info/srcML/literal" <<< "$input"

check 3<<< "$input"

srcml2src --prefix="http://www.sdml.info/srcML/literal" <<< "$input"

check 3<<< "$input"

srcml2src -p "http://www.cs.uakron.edu/~collard/foo" <<< "$input"

check 3<<< "$input"

srcml2src --prefix "http://www.cs.uakron.edu/~collard/foo" <<< "$input"

check 3<<< "$input"

srcml2src --prefix="http://www.cs.uakron.edu/~collard/foo" <<< "$input"

check 3<<< "$input"

# test
define input <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns:cpp="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp"/>
	INPUT

srcml2src -p "http://www.sdml.info/srcML/src" <<< "$input"

check 3<<< "cpp"

srcml2src --prefix "http://www.sdml.info/srcML/src" <<< "$input"

check 3<<< "cpp"

srcml2src --prefix="http://www.sdml.info/srcML/src" <<< "$input"

check 3<<< "cpp"

srcml2src -p "http://www.sdml.info/srcML/cpp" <<< "$input"

check 3<<< ""

srcml2src --prefix "http://www.sdml.info/srcML/cpp" <<< "$input"

check 3<<< ""

srcml2src --prefix="http://www.sdml.info/srcML/cpp" <<< "$input"

check 3<<< ""

# test
##
# no xml declaration

echo -n "" | src2srcml -l C++ --no-xml-declaration
INPUT

check 3<<- 'STDOUT'
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
	
# test
##
# no namespace declaration

echo -n "" | src2srcml -l C++ --no-namespace-decl
INPUT

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++"/>
	STDOUT

# test
##
# check missingfile

src2srcml foo.c

check 3<<- 'STDOUT'
	STDOUT
	src2srcml: Unable to open file foo.c
	STDERR

src2srcml abc.c

check 3<<- 'STDOUT'
	STDOUT
	src2srcml: Unable to open file abc.c
	STDERR

src2srcml ../src/foo.c

check 3<<- 'STDOUT'
	STDOUT
	src2srcml: Unable to open file ../src/foo.c
	STDERR


# test
##
# check correct language based on file extension

src2srcml emptysrc/empty.c

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>
	STDOUT

src2srcml emptysrc/empty.h

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"/>
	STDOUT

src2srcml emptysrc/empty.c.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>
	STDOUT


echo -n "" | src2srcml emptysrc/empty.h.gz

check 3<<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"/>
	STDOUT

# check c++ file extensions

src2srcml emptysrc/empty.cpp

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>
	STDOUT

src2srcml emptysrc/empty.cc

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.cxx

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"
	STDOUT

echo -n "" | src2srcml emptysrc/empty.c++

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"
	STDOUT

echo -n "" | src2srcml emptysrc/empty.hpp

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"
	STDOUT

echo -n "" | src2srcml emptysrc/empty.hh

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.hxx

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"
	STDOUT

echo -n "" | src2srcml emptysrc/empty.h++

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"
	STDOUT

echo -n "" | src2srcml emptysrc/empty.tcc

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.t"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.cpp.gz

<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	STDOUT
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>

echo -n "" | src2srcml emptysrc/empty.cc.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.cxx.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.c++.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.hpp.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.hh.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.hxx.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.h++.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.h"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.tcc.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.t"/>
	STDOUT

define emptyjava 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="C" filename="emptysrc/empty.java"/>
	STDOUT

# check java file extension
echo -n "" | src2srcml emptysrc/empty.java

check 3<<< "$emptyjava"

echo -n "" | src2srcml emptysrc/empty.java.gz

check 3<<< "$emptyjava"

define emptyaj 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="C" filename="emptysrc/empty.aj"/>
	STDOUT

echo -n "" | src2srcml emptysrc/empty.aj.gz

check 3<<< "$emptyaj"

# test
##
# Test output options

# src2srcml
define sfile <<- 'STDOUT'
	a;
	STDOUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define output <<- 'STDOUT'
	xmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	INPUT
	</unit>
	STDOUT

f = open(sub/a.cpp 'w')
f.write(sfile)
f.close()

src2srcml sub/a.cpp' '--output' sub/a.cpp.xml
validate(open(sub/a.cpp.xml 'r').read() fxmlfile)
src2srcml sub/a.cpp' '--output=sub/a.cpp.xml' ""

validate(open(sub/a.cpp.xml 'r').read() fxmlfile)

src2srcml sub/a.cpp' -o sub/a.cpp.xml
validate(open(sub/a.cpp.xml 'r').read() fxmlfil)



if sys.platform != 'cygwin' :

	src2srcml -l C++ '-' -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() sxmlfile)
	src2srcml -l C++ -o sub/a.cpp.xml sfile

	validate(open(sub/a.cpp.xml 'r').read() sxmlfile)



# non-windows
src2srcml -l C++ '-' '--output' '/dev/stdout' <<< "$sfile"

check 3<<< "$sxmlfile"

# test
# srcml2src

srcml2src sub/a.cpp.xml --output sub/a.cpp

check sub/a.cpp "$sfile"

srcml2src sub/a.cpp.xml --output=sub/a.cpp

check sub/a.cpp "$sfile"

srcml2src sub/a.cpp.xml -o sub/a.cpp

check sub/a.cpp "$sfile"

srcml2src '-' -o sub/a.cpp <<< "$sxmlfile"

check sub/a.cpp "$sfile"

srcml2src -o sub/a.cpp <<< "$sxmlfile"

check sub/a.cpp "$sfile"


# non-windows
if platform.system() != "Windows" :
	srcml2src '-' '--output' '/dev/stdout' sxmlfile sfile)
	srcml2src '-' '--output=/dev/stdout' sxmlfile sfile)
	srcml2src '-' -o '/dev/stdout' sxmlfile sfile)

# test
##
# Test src2srcml options with files

sfile1 = ""

sfile2 = STDOUT
b;
	STDOUT

f = open(sub/a.cpp 'w')
f.write(sfile1)
f.close()

# test
##
# empty with debug
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
	STDOUT

define output <<- 'STDOUT'
	?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++" filename=sub/a.cpp/>
	STDOUT

src2srcml sub/a.cpp -g fsrcml

src2srcml sub/a.cpp --debug fsrcml

src2srcml -l C++ -g -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -g sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

##
# language flg
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp/>
	INPUT

src2srcml sub/a.cpp -l "C++"

check 3<<< "$fsrcml"

src2srcml sub/a.cpp --language "C++"

check 3<<< "$fsrcml"

src2srcml sub/a.cpp --language="C++"

check 3<<< "$fsrcml"

src2srcml -l C++ -o sub/a.cpp.xml <<< "$sfile1"

check sub/a.cpp.xml "$srcml"

src2srcml -l C++ sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml "$fsrcml"

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename=sub/a.cpp/>
	INPUT

src2srcml sub/a.cpp' -l --language "C" "" fsrcml)
src2srcml sub/a.cpp' -l --language "C" "" fsrcml)
src2srcml sub/a.cpp' -l --language "C" "" fsrcml)
src2srcml -l 'C' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -l 'C' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="Java"/>
	STDOT
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="Java" filename=sub/a.cpp/>
	STDOUT
INPUT
src2srcml sub/a.cpp' -l --language "Java" "" fsrcml)
src2srcml sub/a.cpp' -l --language "Java" "" fsrcml)
src2srcml -l 'Java' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -l 'Java' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

##
# filename flag
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes">
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo.cpp"/>
	INPUT

define output <<- 'STDOUT'
	f<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo.cpp"/>
	INPUT
src2srcml sub/a.cpp' -f --filename "foo.cpp" "" fsrcml)
src2srcml sub/a.cpp' -f --filename "foo.cpp" "" fsrcml)
src2srcml sub/a.cpp' -f --filename "foo.cpp" "" fsrcml)
src2srcml -l C++ -f 'foo.cpp' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -f 'foo.cpp' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

##
# directory flag
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar"/>
	STDOUT
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename=sub/a.cpp/>
	INPUT
src2srcml sub/a.cpp' -d --directory "bar" "" fsrcml)
src2srcml sub/a.cpp' -d --directory "bar" "" fsrcml)
src2srcml sub/a.cpp' -d --directory "bar" "" fsrcml)
src2srcml -l C++ -d 'bar' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -d 'bar' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

##
# version flag
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes">
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" version="1.0"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp version="1.0"/>
	INPUT
src2srcml sub/a.cpp' -s --src-version "1.0" "" fsrcml)
src2srcml sub/a.cpp' -s --src-version "1.0" "" fsrcml)
src2srcml sub/a.cpp' -s --src-version "1.0" "" fsrcml)
src2srcml -l C++ -s '1.0' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -s '1.0' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



##

# xml encoding flag
# test
<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	STDOUT

f<?xml vrsion="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp/>
	STDOUT
src2srcml sub/a.cpp' -x --encoding "ISO-8859-1" "" fsrcml)
src2srcml sub/a.cpp' -x --encoding "ISO-8859-1" "" fsrcml)
src2srcml sub/a.cpp' -x --encoding "ISO-8859-1" "" fsrcml)
src2srcml -l C++ -x 'ISO-8859-1' -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml -x 'ISO-8859-1' sub/a.cpp-o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



##

# text encoding flag
# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes">
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp/>
	INPUT
src2srcml -t --src-encoding "ISO-8859-1" sfile1 srcml)
src2srcml -t --src-encoding "ISO-8859-1" sfile1 srcml)
src2srcml -t --src-encoding "ISO-8859-1" sfile1 srcml)
src2srcml sub/a.cpp' -t --src-encoding "ISO-8859-1" "" fsrcml)
src2srcml sub/a.cpp' -t --src-encoding "ISO-8859-1" "" fsrcml)
src2srcml sub/a.cpp' -t --src-encoding "ISO-8859-1" "" fsrcml)
src2srcml --src-encoding "ISO-8859-1" sub/a.cpp "" fsrcml)
src2srcml -l C++ --src-encoding "ISO-8859-1" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --src-encoding "ISO-8859-1" sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# neste 


sfile1 = STDOUT
a;
	STDOUT

sfile2 = STDOUT
b;
	STDOUT


define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
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

	<src:unit>
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	</src:unit>
	STDOUT

# f = open(sub/a.cpp 'w')
# f.write(sfile1)
# f.close()

# f = open('sub/b.cpp' 'w')
# f.write(<<< "b;"
# f.close()

src2srcml --archive sub/a.cpp -o sub/a.cpp.xml
readfile input sub/a.cpp.xml').read() <<< "$nestedfile1)"
src2srcml sub/a.cpp' 'sub/b.cpp' -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() <<< "$nestedfile)"
src2srcml --xmlns:src=http://www.sdml.info/srcML/src' sub/a.cpp 'sub/b.cpp' -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() <<< "$nestedfilesrc)"


# files from
define output <<- 'STDOUT'
	<<< "$nestedfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
	STDOUT

filelist = STDOUT
sub/a.cpp
# fff
sub/b.cpp
	STDOUT

f = open('filelistab' 'w')
f.write("\nsub/a.cpp\nsub/b.cpp\n\n")
f.close()

src2srcml --files-from "filelistab" -o sub/a.cpp.xml
validate(open(sub/a.cpp.xml 'r').read() <<< "$nestedfile)"


##

# xmlns options

sfile1 = ""

sfile2 = STDOUT
b;
	STDOUT

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp/>
	INPUT

f = open(sub/a.cpp 'w')
f.write(sfile1)
f.close()

# separate
src2srcml --xmlns="http://www.sdml.info/srcML/src" sub/a.cpp "" fsrcml)
src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)


src2srcml --xmlns="http://www.sdml.info/srcML/src" sub/a.cpp -o sub/a.cpp.xml


validate(open(sub/a.cpp.xml 'r').read() fsrcml)

src2srcml --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp "" fsrcml)



src2srcml -l C++ --xmlns:cpp="http://www.sdml.info/srcML/cpp" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)


src2srcml --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)




# multiple
src2srcml --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp "" 

fsrcml)
src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" -o 'sub/a
.cpp.xml' sfile1

validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp -o 
sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)


# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++" filename=sub/a.cpp/>
	INPUT


# separate
src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" "--

xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp "" fsrcml)src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" 
--xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" "--

xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xmlvalidate(open(sub/a.cpp.xml 'r').read() fsrcm)




# multc2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" 'sub/a.

src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" '

validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" 'sub/a.
cpp' -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)


src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp "" fsrcml)src2srcml -l C++ --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sfile1validate(open(sub/a.cpp.xml 'r').read() srcml)

src2srcml --debug --xmlns="http://www.sdml.info/srcML/src" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xmlvalidate(open(sub/a.cpp.xml 'r').read() fsrcml)




src2srcml --debug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" 
'sub/a.c fsrcml)
src2srcm++ --debug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" -o sub/a.cpp.xml sfile1
src2srcmbug --xmlns:cpp="http://www.sdml.info/srcML/cpp" --xmlns:err="http://www.sdml.info/srcML/srcerr" sub/a.cpp -o sub/a.cpp.xml
validate(open(sub/a.cpp.xml 'r').read() fsrcml)

# test

 output <<- 'STDOUT'

	version="1.0" encoding="UTF-8" standalone="yes"?>
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	STDOUT<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp/>
	INPUT
src2srcml --xmlns:src=http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp" sub/a.cpp "" fsrcml)
src2srcml -l C++ --xmlns:src=http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp" -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --xmlns:src=http://www.sdml.info/srcML/src" --xmlns="http://www.sdml.info/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



##

# no xml declaration
# test
srcml = <unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

fsrcml  <unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	STDOUT

src2srcml --no-xml-declaration
src2srcml -l C++ --no-xml-declaration
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --no-xml-declaration
validate(open(sub/a.cpp.xml 'r').read() fsrcml)


##

# no namespace declaration
# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++"/>
	STDOT
INPUT
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++" filename=sub/a.cpp/>
	STDOUT
INPUT
src2srcml --no-namespace-decl sub/a.cpp "" fsrcml)
src2srcml -l C++ --no-namespace-decl -o sub/a.cpp.xml sfile1
validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --no-namespace-decl sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



##

# cpp markup else

cpp_src = STDOUT
#if A
brea;
#else
return;
#endif
	STDOUT

define output <<- 'STDOUT'
	cpp_marked_<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
INPUT
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
<return>return;</return>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
	STDOUT

define output <<- 'STDOUT'
	cpp_marked_<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	INPUT
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
<return>return;</return>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
	STDOUT

f = open(sub/a.cpp 'w')
f.write(cpp_src)
f.close()

src2srcml sub/a.cpp' "" fcpp_marked_srcml)
if sys.platform != 'cygwin':
	src2srcml -l C++ -o sub/a.cpp.xml cpp_src
	validate(open(sub/a.cpp.xml 'r').read() cpp_marked_srcml)
src2srcml sub/a.cpp' -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fcpp_marked_srcml)



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

src2srcml sub/a.cpp' "" fcpp_textonly_srcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ -o sub/a.cpp.xml cpp_if0
	validate(open(sub/a.cpp.xml 'r').read() cpp_textonly_srcm)
src2srcml sub/a.cpp' -o sub/a.cpp.xml

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
# test compression tool

define sfile <<< "a;"

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	</unit>
	STDOUT

# # src2srcml
# if platform.system() != "Windows" :

#	 f = open(sub/a.cpp 'w')
#	 f.write(sfile)
#	 f.close()
#	 src2srcml -n sub/a.cpp -o 'sub/a.cpp.xml.gz' ""
#	 'gunzip' '-c' 'sub/a.cpp.xml.gz' "" fxmlfile)
#	 src2srcml --archive sub/a.cpp -o 'sub/a.cpp.xml.gz' ""
#	 'gunzip' '-c' 'sub/a.cpp.xml.gz' "" fxmlfil)
#	 if sys.platform != 'cygwin' :
#	 src2srcml -l C++ -n -o 'sub/a.cpp.xml.gz' sfile
#	 'gunzip' '-c' 'sub/a.cpp.xml.gz' "" sxmlfile)


# # srcml2src

# if platform.system() != "Windows" :

#	 f = open(sub/a.cpp.xml 'w')
#	 f.write(fxmlfile)
#	 f.close()

#	 srcml2src -n sub/a.cpp.xml -o 'sub/a.cpp.gz' ""
#	 'gunzip' '-c' 'sub/a.cpp.gz' <<< "a")
#	 srcml2src --archive sub/a.cpp.xml -o 'sub/a.cpp.gz' ""

#	 'gunzip' '-c' 'sub/a.cpp.gz' <<< "a")
#	 if sys.platform != 'cygwin' :

#	 srcml2src -n -o 'sub/a.cpp.gz' fxmlfile
#	 'gunzip' '-c' 'sub/a.cpp.gz' <<< "a")

# test input file is gzipped

define sfile <<< "a;"

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT

define fxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp.gz">
	</unit>
	STDOUT

if platform.system() != "Windows" :

	f = open('sub/a.cpp.gz' 'r')
	gzipped = f.read()
	f.close()

	src2srcml -l C++ gzipped sxmlfile)
	src2srcml sub/a.cpp.gz' "" fxmlfile)
	if sys.platform != 'cygwin' :
	src2srcml -l C++ -o sub/a.cpp.xml gzipped
	validate(open(sub/a.cpp.xml 'r').read() sxmlfile)
	src2srcml sub/a.cpp.gz' -o sub/a.cpp.xml

	validate(open(sub/a.cpp.xml 'r').read() fxmlfile)

	

	f = open('sub/a.cpp.xml.gz' 'r')

	gzipped = f.read()
	f.close()

	srcml2src gzipped sfile)
	srcml2src 'sub/a.cpp.xml.gz' <<< "a")
	if sys.platform != 'cygwin' :
	srcml2src -o sub/a.cpp gzipped
	validate(open(sub/a.cpp 'r').read() sfile)
	srcml2src 'sub/a.cpp.xml.gz' -o sub/a.cpp ""
	validate(open(sub/a.cpp 'r').read() sfile)

##
# src2srcml Markup Extensions
# test
createfile sub/a.cpp ""

define sfilelit <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" language="C++" filename=sub/a.cpp/>
	STDOUT

echo -n "" | src2srcml -l C++ --literal

check sub/a.cpp.xml 3<<< "$sfilelit"

src2srcml --literal sub/a.cpp 

check sub/a.cpp.xml 3<<< "$sfilelit"

echo -n "" | src2srcml -l C++ --literal -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfilelit"

src2srcml --literal sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfilelit"

# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" language="C++">
	INPUT

define fsrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" language="C++" filename=sub/a.cpp/>
	STDOUT

echo -n "" | src2srcml -l C++ --operator

check 3<<< "$fsrcml"

src2srcml --operator sub/a.cpp

check 3<<< "$fsrcml"

echo -n "" | src2srcml -l C++ --operator -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$fsrcml"

src2srcml --operator sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$fsrcml"

# test
define sfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++">
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename=sub/a.cpp/>
	INPUT

echo -n "" | src2srcml -l C++ --modifier

check 3<<< "$fsrcml"

echo -n "" | src2srcml --modifier sub/a.cpp 

check 3<<< "$fsrcml"

src2srcml -l C++ --modifier -o sub/a.cpp.xml sfile

check sub/a.cpp.xml "$srcml"

src2srcml --modifier sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml "$fsrcml"

# test

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" language="C++">
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" language="C++" filename=sub/a.cpp/>
	STDOUT

echo -n "" | src2srcml -l C++ --literal --operator

check 3<<< "$srcml"

src2srcml --literal --operator sub/a.cpp

check 3<<< "$srcml"

echo -n "" | src2srcml -l C++ --literal --operator -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfile"

src2srcml --literal --operator sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$sfile"

src2srcml -l C++ --operator --literal sfile srcml)

src2srcml --operator --literal sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --operator --literal -o sub/a.cpp.xml sfie
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --operator --literal sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename=sub/a.cpp/>
	INPUT

src2srcml -l C++ --literal --modifier sfile srcml)
src2srcml --literal --modifier sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --literal --modifier -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --literal --modifier sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --modifier --literal sfile srcml)

src2srcml --modifier --literal sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --modifier --literal -o sub/a.cpp.xml sfie
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --modifier --literal sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
	INPUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename=sub/a.cpp/>
	INPUT

src2srcml -l C++ --operator --modifier sfile srcml)
src2srcml --operator --modifier sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --operator --modifier -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --operator --modifier sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --modifier --operator sfile srcml)

src2srcml --modifier --operator sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --modifier --operator -o sub/a.cpp.xml sfie
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --modifier --operator sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp=	STDOUT

define output <<- 'STDOUT'
	f<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp=""	STDOUT

src2srcml -l C++ --literal --operator --modifier sfile srcml)
src2srcml --literal --operator --modifier sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --literal --operator --modifier -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --literal --operator --modifier sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --literal --modifier --operator sfile srcml)

src2srcml --literal --modifier --operator sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --literal --modifier --operator -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --literal --modifier --operator sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --operator --literal --modifier sfile srcml)

src2srcml --operator --literal --modifier sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --operator --literal --modifier -o sub/a.cpp.xml sfile
	 validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --operator --literal --modifier sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --operator --modifier --literal sfile srcml)

src2srcml --operator --modifier --literal sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --operator --modifier --literal -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --operator --modifier --literal sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --modifier --literal --operator sfile srcml)

src2srcml --modifier --literal --operator sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --modifier --literal --operator -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --modifier --literal --operator sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



src2srcml -l C++ --modifier --operator --literal sfile srcml)

src2srcml --modifier --operator --literal sub/a.cpp "" fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --modifier --operator --literal -o sub/a.cpp.xml sfile
	validate(open(sub/a.cpp.xml 'r').read() srcml)
src2srcml --modifier --operator --literal sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# test

##
# srcml2src info and longinfo

info = STDOUTxmlns="http://www.sdml.info/srcML/src"
xmlns:cpp="http://www.sdml.info/srcML/cpp"
encoding="UTF-8"
language="C++"
directory="sub"
filename="a.cpp"
	STDOUT

longinfo = STDOUTxmlns="http://www.sdml.info/srcML/src"
xmlns:cpp="http://www.sdml.info/srcML/cpp"
encoding="UTF-8"
language="C++"
directory="sub"
filename="a.cpp"
units="1"
	STDOUT

longinfonested = STDOUTxmlns="http://www.sdml.info/srcML/src"
encoding="UTF-8"
units="2"
	STDOUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
	INPUT
</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml << "$sxmlfile1"

srcml2src -i <<< "$sxmlfile"

check 3<<< "$info"

srcml2src --info <<< "$sxmlfile"

check 3<<< "$info"

srcml2src sub/a.cpp.xml -i

check 3<<< "$info"

srcml2src sub/a.cpp.xml --info

check 3<<< "$info"

srcml2src --longinfo <<< "$sxmlfile"

check 3<<< "$longinfo"

srcml2src sub/a.cpp.xml --longinfo

check 3<<< "$longinfo"

createfile sub/a.cpp.xml <<< "$nestedfile"

srcml2src -i --longinfo <<< "$nestedfile longinfonested)"
srcml2src -i --longinfo <<< "$nestedfile longinfonested)"
srcml2src -i --longinfo <<< "$nestedfile longinfonested)"
srcml2src sub/a.cpp.xml -i --longinfo longinfonested)
srcml2src sub/a.cpp.xml -i --longinfo longinfonested)
srcml2src sub/a.cpp.xml -i --longinfo longinfonested)



# test

##
# test extract all command
define sfile1 <<< "a;"

define sfile2 <<< "b;"

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" language="C++" filename="sub/a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

f = open(sub/a.cpp.xml 'w')
f.write(srcml)
f.close()


if platform.system() != "Windows" :
	os.system("rm -f sub/a.cpp")
else :
	os.system("del sub\\a.cpp")

#remove
srcml2sr --to-dir=. sub/a.cpp.xml

check sub/a.cpp <<< "a;"

createfile sub/a.cpp.xml <<< "$nestedfile"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml2src --to-dir=sub/a.cpp.xml

check sub/a.cpp 3<<< "a;"
check sub/b.cpp 3<<< "b;"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml2src -a '.' sub/a.cpp.xml

check sub/a.cpp 3<<< "a;"
check sub/b.cpp 3<<< "b;"

# test
##
# xml flag

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	</unit>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

srcml2src --xml srcml srcml)

srcml2src --xml sub/a.cpp.xml

srcml2src --xml -o sub/a.cpp.xml srcml

check sub/a.cpp.xml <<< "$srcml"

srcml2src --xml sub/a.cpp.xml -o sub/b.cpp.xml

check sub/a.cpp.xml <<< "$srcml"

# test
##
# no xml declaration srcml2src
define srcml <<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT


createfile sub/a.cpp.xml "$srcml"

srcml2src --xml --no-xml-declaration

check sub/a.cpp.xml 3<<< "$srcmlout"

srcml2src --no-xml-declaration <<< "$srcml"

check sub/b.cpp.xml 3<<< "$srcmlout"

# test
define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	</unit>
	STDOUT


createfile sub/a.cpp.xml <<< "$srcml"

srcml2src --xml --no-xml-declaration

check sub/a.cpp.xml 3<<< "$srcmlout"

srcml2src --no-xml-declaration

check sub/a.cpp.xml 3<<< "$srcmlout"

# test
##
# no namespace declaration

<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

define output <<- 'STDOUT'
	srcmlout = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++"/>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

srcml2src --xml --no-namespace-decl srcml srcmlout)
srcml2src --xml --no-namespace-decl 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --xml --no-namespace-decl -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --xml --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


srcml2src --no-namespace-decl --xml srcml srcmlout)
srcml2src --no-namespace-decl --xml 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --no-namespace-decl --xml -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --no-namespace-decl --xml sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


srcml2src --no-namespace-decl srcml srcmlout)
srcml2src --no-namespace-decl 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --no-namespace-decl -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


# test
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
</unit>
	STDOUT

define output <<- 'STDOUT'
	srcmlout = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit language="C++">
	</unit>
INPUT

f = open(sub/a.cpp.xml 'w')
f.write(srcml)
f.close()


srcml2src --xml --no-namespace-decl srcml srcmlout)
srcml2src --xml --no-namespace-decl 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --xml --no-namespace-decl -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --xml --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


srcml2src --no-namespace-decl --xml srcml srcmlout)
srcml2src --no-namespace-decl --xml 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --no-namespace-decl --xml -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --no-namespace-decl --xml sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


srcml2src --no-namespace-decl srcml srcmlout)
srcml2src --no-namespace-decl 'sub/a.cpp.xml <<INPUTout)
if sys.platform != 'cygwin' :
	srcml2src --no-namespace-decl -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml).read() srcmlout)
srcml2src --no-namespace-decl sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml).read() srcmlout)


# test
##
# Help and version

src2srcml -h
src2srcml --help

srcml2src -V
srcml2src --version

# test
##
# Test order of metadata option order

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" version="1.0"/>
	INPUT

options = [-l -d -f -s -x
	   -l -d -f -s -x

values = ['language="C++"\n' 'directory="sub"\n' 'filename="a.cpp"\n' 'src-version="1.0"\n' 'encoding="UTF-8"\n'
	  'language="C++"\n' 'directory="sub"\n' 'filename="a.cpp"\n' 'src-version="1.0"\n' 'encoding="UTF-8"\n'

index = 0
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

index += 1
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

index += 1
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

index += 1
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

index += 1
srcml2src options[index + options[index + 1[1 + options[index + 2[1 + options[index + 3[1 + options[index + 4[1 srcml values[index + values[index + 1 + values[index + 2 + values[index + 3 + values[index + 4)

# test
##
# Testing for verbose

define sfile <<< "a;"

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT
createfile sub/a.cpp "$sfile"

createfile sub/a.cpp.xml "$sxmlfile"
 
# src2srcml
if platform.system() != "Windows" and sys.platform != 'cygwin' :

	globals()["test_count" += 1
	globals()["test_line" = os.path.basename(src2srcml) + ' ' + --verbose
	print test_count os.path.basename(src2srcml) + ' ' + --verbose
	line = src2srcml --verbose -l C++ sfile)
	execute(['grep' srcencoding + xmlencoding line)

	globals()["test_count" += 1
	globals()["test_line" = os.path.basename(src2srcml) + ' ' + --verbose + ' sub/a.cpp'
	print test_count os.path.basename(src2srcml) + ' ' + --verbose + ' sub/a.cpp'
	line = src2srcml --verbose sub/a.cpp
	execute(['grep' srcencoding + xmlencoding line)

# srcml2src
if platform.system() != "Windows" and sys.platform != 'cygwin' :

	globals()["test_count" += 1
	globals()["test_line" = os.path.basename(srcml2src) + ' ' + --verbose
	print test_count os.path.basename(srcml2src) + ' ' + --verbose
	line = execute([srcml2src --verbose sxmlfile)
	execute(['grep' xmlencoding + srcencoding line)

	globals()["test_count" += 1
	globals()["test_line" = os.path.basename(srcml2src) + ' ' + --verbose + ' sub/a.cpp.xml'
	print test_count os.path.basename(srcml2src) + ' ' + --verbose + ' sub/a.cpp.xml'
	line = execute([srcml2src --verbose 'sub/a.cpp.xml'
	execute(['grep' xmlencoding + srcencoding line)

# test expression_mode

define sxmlfile <<-'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr><name>a</name></expr>
	</unit>
	STDOUT

define fsxmlfile <<- 'STDOUT'
	<?xml vesion="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	<expr><name>a</name></expr>
	</unit>
	STDOUT

echo -n "a" >> sub/a.cpp

src2srcml -e <<< "a"

check 3<<< "$sxmlfile"

src2srcml --expression <<< "a"

check 3<<< "$sxmlfile

src2srcml sub/a.cpp -e

check 3<<< "$fsxmlfile"

src2srcml sub/a.cpp --expression

check 3<<< "$fsxmlfile"


#src2srcml -l C++ --expression -o sub/a.cpp.xml sfile
#validate(open(sub/a.cpp.xml 'r').read() sxmlfile)
#src2srcml --expression sub/a.cpp -o sub/a.cpp.xml

#validate(open(sub/a.cpp.xml 'r').read() fsxmlfile)



##

# Test Query and Transformation Options

# xpath

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
INPUT
xpath_error = STDOUTsrcml2src: Start tag expected '<' not found in '-'
	STDOUT

define output <<- 'STDOUT'
	xpath = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT

</unit>
	STDOUT

define output <<- 'STDOUT'
	xpath_empty = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src"/>
	STDOUT
INPUT
file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


if sys.platform != 'cygwin' :
	checkError([srcml2src --xpath=/src:unit' "" xpath_error)
	srcml2src --xpath=/src:unit

	check 4<<< "2"

srcml2src --xpath=/src:unit' srcml xpath)
srcml2src --xpath=/src:unit' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=/src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=/src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=//src:unit srcml xpath)
scml2src --xpath=//src:unit 'sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=//src:unit -o 'sub/b.cp.xml' srcml
	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=//src:unit 'sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)

srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=src:unit' srcml xpath_empty)
srcml2src --xpath=src:unit' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --xpath=src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --xpath=src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

# test
# xpath apply root

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
INPUT
xpath_error = STDOUTsrcml2src: Start tag expected '<' not found in '-'
	STDOUT

define output <<- 'STDOUT'
	xpath = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT

</unit>
	STDOUT

define output <<- 'STDOUT'
	xpath_empty = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src"/>
	STDOUT
INPUT
file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


if sys.platform != 'cygwin' :
	checkError([srcml2src --apply-root --xpath=/src:unit' "" xpath_error)
	srcml2src --apply-root --xpath=/src:unit

	check 4<<< "2"

srcml2src --apply-root --xpath=/src:unit' srcml xpath)
srcml2src --apply-root --xpath=/src:unit' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --apply-root --xpath=/src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --apply-root --xpath=/src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --apply-root --xpath srcml

check 4<<< "1"
srcml2src --apply-root --xpath=' srcml

check 4<<< "1"

srcml2src --apply-root --xpath=//src:unit srcml xpath)
scml2src --apply-root --xpath=//src:unit 'sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --apply-root --xpath=//src:unit -o 'sub/b.cp.xml' srcml
	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --apply-root --xpath=//src:unit 'sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)

srcml2src --apply-root --xpath srcml

check 4<<< "1"
srcml2src --apply-root --xpath=' srcml

check 4<<< "1"

srcml2src --apply-root --xpath=src:unit' srcml xpath_empty)
srcml2src --apply-root --xpath=src:unit' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --apply-root --xpath=src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --apply-root --xpath=src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --apply-root --xpath srcml

check 4<<< "1"

srcml2src --apply-root --xpath='' srcml

check 4<<< "1"

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" item="2"><expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>
	</unit>
	STDOUT

createfile sub/a.cpp.xml $output

srcml2src --apply-root --xpath=/src:unit #srcml_nested xpath_nested)

srcml2src --apply-root --xpath=/src:unit #sub/a.cpp.xml xpath_nested)

srcml2src --apply-root --xpath=/src:unit -o sub/b.cpp.xml #srcml_nested

readfile input sub/b.cpp.xml

srcml2src --apply-root --xpath=/src:unit #' sub/a.cpp.xml -o sub/b.cpp.xml ""

readfile input sub/b.cpp.xml

srcml2src --apply-root --xpath=//src:unit srcml_nested xath_nested_recursive)
srcml2src --apply-root --xpath=//src:unit 'sub/a.cpp.xml xpath_nested_recursive)
srcml2src --apply-root --xpath=//src:unit -o 'sub/b.cp.xml' srcml_nested
validate(open(sub/b.cpp.xml 'r').read() xpath_nested_recursive)
srcml2src --apply-root --xpath=//src:unit 'sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_nested_recursive)

srcml2src --apply-root --xpath=/src:unit/src:unit[1/src:expr_stmt' srcml_nested xpath_single_expr_stmt)
srcml2src --apply-root --xpath=/src:unit/src:unit[1/src:expr_stmt' sub/a.cpp.xml xpath_single_expr_stmt)

srcml2src --apply-root --xpath=/src:unit/src:unit[1/src:expr_stmt' -o sub/b.cpp.xml srcml_nested

validate(open(sub/b.cpp.xml 'r').read() xpath_single_expr_stmt)
srcml2src --apply-root --xpath=/src:unit/src:unit[1/src:expr_stmt' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_single_expr_stmt)


srcml2src --apply-root --xpath=//src:expr_stmt' srcml_nested xpath_nested_expr_stmt)
srcml2src --apply-root --xpath=//src:expr_stmt' sub/a.cpp.xml xpath_nested_expr_stmt)
srcml2src --apply-root --xpath=//src:expr_stmt' -o sub/b.cpp.xml srcml_nested

validate(open(sub/b.cpp.xml 'r').read() xpath_nested_expr_stmt)
srcml2src --apply-root --xpath=//src:expr_stmt' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_nested_expr_stmt)


# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src"/>
	STDOUT

createfile sub/a.cpp.xml $output

srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' srcml xpath)
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --xpath srcml

check 4<<< "1"

srcml2src --xpath=''

check 4<<< "1"

srcml2src --xpath=//src:name' srcml xpath)
srcml2src --xpath=//src:name' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=//src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=//src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=src:name' srcml xpath_empty)
srcml2src --xpath=src:name' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --xpath=src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --xpath=src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
INPUT
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/src" xmlns:bar="http://www.cs.uakron.edu/~collard/bar" language="Java">
<cpp:expr_stmt><cpp:expr><cpp:name>b</cpp:name></cpp:expr>;</cpp:expr_stmt>
</unit>

</unit>
	STDOUT

define output <<- 'STDOUT'
	xpath = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
INPUT

<unit xmlns:cpp="http://www.sdml.info/srcML/src" xmlns:bar="http://www.cs.uakron.edu/~collard/bar" language="Java" item="1"><cpp:name>b</cpp:name></unit>

</unit>
	STDOUT

define output <<- 'STDOUT'
	xpath_empty = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo"/>
	STDOUT
INPUT
file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' srcml xpath)
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=/src:unit/src:expr_stmt/src:expr/src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=//src:name' srcml xpath)
srcml2src --xpath=//src:name' sub/a.cpp.xml xpath)
if sys.platform != 'cygwin' :
	srcml2src --xpath=//src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath)
srcml2src --xpath=//src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=src:name' srcml xpath_empty)
srcml2src --xpath=src:name' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --xpath=src:name' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --xpath=src:name' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

# test other_prefixes

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
<unit language="Java">
<foo:a/>
</unit>

<unit xmlns:bar="http://www.cs.uakron.edu/~collard/bar" language="Java">
<bar:b/>
</unit>

</unit>
	STDOUT

define xpathempty <<-'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo"/>
	STDOUT

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


srcml2src --xpath=/src:unit' srcml srcml)
echo -n "" | srcml2src --xpath=/src:unit' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xpath=/src:unit' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xpath=/src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=//src:unit srcml srcml)
srcml2src --xpath=//src:unit 'sub/a.cpp.xml<<< ""
if sys.platform != 'cygwin' :
	srcml2src --xpath=//src:unit -o 'sub/b.cp.xml' srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xpath=//src:unit 'sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)

srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=src:unit' srcml xpath_empty)
srcml2src --xpath=src:unit' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --xpath=src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --xpath=src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
INPUT
<foo:a/>
</unit>

<unit xmlns:bar="http://www.cs.uakron.edu/~collard/foo" language="Java">
<foo:a/><bar:b/>
</unit>

</unit>
	STDOUT

define output <<- 'STDOUT'
	xpath_empty = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo"/>
	STDOUT
INPUT
file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


srcml2src --xpath=/src:unit' srcml srcml)
echo -n "" | srcml2src --xpath=/src:unit' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xpath=/src:unit' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xpath=/src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=//src:unit srcml srcml)
scml2src --xpath=//src:unit 'sub/a.cpp.xml<<< ""
if sys.platform != 'cygwin' :
	srcml2src --xpath=//src:unit -o 'sub/b.cp.xml' srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xpath=//src:unit 'sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)

srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

srcml2src --xpath=src:unit' srcml xpath_empty)
srcml2src --xpath=src:unit' sub/a.cpp.xml xpath_empty)
if sys.platform != 'cygwin' :
	srcml2src --xpath=src:unit' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xpath_empty)
srcml2src --xpath=src:unit' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xpath_empty)


srcml2src --xpath srcml

check 4<<< "1"
srcml2src --xpath=' srcml

check 4<<< "1"

# xslt and param

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
INPUT
xslt_error = STDOUTsrcml2src: Start tag expected '<' not found in '-'
	STDOUT

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


# xslt

if sys.platform != 'cygwin' :
	checkError([srcml2src --xslt + '=copy.xsl' "" xslt_error)
	srcml2src --xslt + '=copy.xsl

	check 4<<< "2"

srcml2src --xslt + '=copy.xsl' srcml srcml)
echo -n "" | srcml2src --xslt + '=copy.xsl' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=copy.xsl' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xslt + '=copy.xsl' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xslt srcml

check 4<<< "1"
srcml2src --xslt + '=' srcml

check 4<<< "1"

# xslt apply root

if sys.platform != 'cygwin' :
	checkError([srcml2src --apply-root --xslt + '=copy.xsl' "" xslt_error)
	srcml2src --apply-root --xslt + '=copy.xsl

	check 4<<< "2"

srcml2src --apply-root --xslt + '=copy.xsl' srcml srcml)
echo -n "" | srcml2src --apply-root --xslt + '=copy.xsl' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --apply-root --xslt + '=copy.xsl' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --apply-root --xslt + '=copy.xsl' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --apply-root --xslt srcml

check 4<<< "1"
srcml2src --apply-root --xslt + '=' srcml

check 4<<< "1"

# param

srcml2src --xslt + '=copy.xsl' --xpathparam 'NAME=VALUE' srcml srcml)
echo -n "" | srcml2src --xslt + '=copy.xsl' --xpathparam 'NAME=VALUE' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=copy.xsl' --xpathparam 'NAME=VALUE' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xslt + '=copy.xsl' --xpathparam 'NAME=VALUE' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xslt + '=copy.xsl' --xpathparam srcml

check 4<<< "1"
srcml2src --xslt + '=copy.xsl' --xpathparam "NAME" srcml

check 4<<< "1"

xslt = STDOUTa
	STDOUT

srcml2src --xslt + '=param.xsl' --xpathparam 'name="a"' srcml xslt)
srcml2src --xslt + '=param.xsl' --xpathparam 'name="a"' sub/a.cpp.xml xslt)
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=param.xsl' --xpathparam 'name="a"' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xslt)
srcml2src --xslt + '=param.xsl' --xpathparam 'name="a"' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xslt)


srcml2src --xslt + '=copy.xsl' --xpathparam srcml

check 4<<< "1"
srcml2src --xslt + '=copy.xsl' --xpathparam "name" srcml

check 4<<< "1"

# stringparam

srcml2src --xslt + '=copy.xsl' --param 'NAME=VALUE' srcml srcml)
echo -n "" | srcml2src --xslt + '=copy.xsl' --param 'NAME=VALUE' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=copy.xsl' --param 'NAME=VALUE' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --xslt + '=copy.xsl' --param 'NAME=VALUE' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


srcml2src --xslt + '=copy.xsl' --param srcml

check 4<<< "1"
srcml2src --xslt + '=copy.xsl' --param "NAME" srcml

check 4<<< "1"

xslt = STDOUTa
	STDOUT

srcml2src --xslt + '=param.xsl' --param 'name=a' srcml xslt)
srcml2src --xslt + '=param.xsl' --param 'name=a' sub/a.cpp.xml xslt)
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=param.xsl' --param 'name=a' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xslt)
srcml2src --xslt + '=param.xsl' --param 'name=a' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xslt)


srcml2src --xslt + '=copy.xsl' --param srcml

check 4<<< "1"
srcml2src --xslt + '=copy.xsl' --param "name" srcml

check 4<<< "1"

# test
# src:archive
# empty test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" filename="a.cpp" language="C++"/>
	INPUT

xslt = STDOUTa.cpp
C++
	STDOUT

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


srcml2src --xslt + '=archive.xsl' srcml xslt)
srcml2src --xslt + '=archive.xsl' sub/a.cpp.xml xslt)
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=archive.xsl' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xslt)
srcml2src --xslt + '=archive.xsl' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xslt)


srcml2src --xslt srcml

check 4<<< "1"
srcml2src --xslt + '=' srcml

check 4<<< "1"

# test
# single file test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" filename="a.cpp" language="C++">
	INPUT
</unit>
	STDOUT

xslt = STDOUTa.cpp
C++
	STDOUT

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


srcml2src --xslt + '=archive.xsl' srcml xslt)
srcml2src --xslt + '=archive.xsl' sub/a.cpp.xml xslt)
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=archive.xsl' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xslt)
srcml2src --xslt + '=archive.xsl' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xslt)


srcml2src --xslt srcml

check 4<<< "1"
srcml2src --xslt + '=' srcml

check 4<<< "1"

# archive test

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit filename="b.cpp" language="Java">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
	STDOUT

xslt = ""

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


srcml2src --xslt + '=archive.xsl' srcml xslt)
srcml2src --xslt + '=archive.xsl' sub/a.cpp.xml xslt)
if sys.platform != 'cygwin' :
	srcml2src --xslt + '=archive.xsl' -o sub/b.cpp.xml srcml

	validate(open(sub/b.cpp.xml 'r').read() xslt)
srcml2src --xslt + '=archive.xsl' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() xslt)


srcml2src --xslt srcml

check 4<<< "1"
srcml2src --xslt + '=' srcml

check 4<<< "1"

# relaxng

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT
INPUT
relaxng_error = STDOUTsrcml2src: Start tag expected '<' not found in '-'
	STDOUT

file = open(sub/a.cpp.xml 'w')
file.write(srcml)
file.close()


if sys.platform != 'cygwin' :
	checkError([srcml2src --relaxng + '=schema.rng' "" relaxng_error)
	srcml2src --relaxng + '=schema.rng

	check 4<<< "2"

srcml2src --relaxng + '=schema.rng' srcml srcml)
echo -n "" | srcml2src --relaxng + '=schema.rng' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --relaxng + '=schema.rng' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --relaxng + '=schema.rng' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


# TODO really make sure this is ok to return ok when no schema supplied.
srcml2src --relaxng srcml

check 4<<< "0"
if sys.platform != 'cygwin' :
	srcml2src --relaxng + '=' srcml

	check 4<<< "0"

# relaxng apply root

if sys.platform != 'cygwin' :
	checkError([srcml2src --apply-root --relaxng + '=schema.rng' "" relaxng_error)
	srcml2src --apply-root --relaxng + '=schema.rng

	check 4<<< "2"

srcml2src --apply-root --relaxng + '=schema.rng' srcml srcml)
echo -n "" | srcml2src --apply-root --relaxng + '=schema.rng' 'sub/a.cpp.xml
if sys.platform != 'cygwin' :
	srcml2src --apply-root --relaxng + '=schema.rng' -o sub/b.cpp.xml srcml
	validate(open(sub/b.cpp.xml 'r').read() srcml)
srcml2src --apply-root --relaxng + '=schema.rng' sub/a.cpp.xml -o sub/b.cpp.xml ""
validate(open(sub/b.cpp.xml 'r').read() srcml)


# TODO really make sure this is ok to return ok when no schema supplied.
srcml2src --apply-root --relaxng srcml

check 4<<< "0"
if sys.platform != 'cygwin' :
	srcml2src --apply-root --relaxng + '=' srcml

	check 4<<< "0"

# position

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="8"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename=sub/a.cpp pos:tabs="8"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --position "" sxmlfile)
src2srcml --position sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --position -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --position sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile


# tabs

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="4"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename=sub/a.cpp pos:tabs="4"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --tabs '4' "" sxmlfile)
src2srcml --tabs '4' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --tabs '4' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --tabs '4' sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile


src2srcml -l C++ --tabs 1)
src2srcml -l C++ --tabs 'a' 14)

# position and tabs

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="2"/>
	INPUT

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename=sub/a.cpp pos:tabs="2"/>
	INPUT

f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml -l C++ --position --tabs '2' "" sxmlfile)
src2srcml --position --tabs '2' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --position --tabs '2' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --position --tabs '2' sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile


src2srcml -l C++ --position --tabs 1)
src2srcml -l C++ --position --tabs 'a' 14)

src2srcml -l C++ --tabs '2' --position "" sxmlfile)
src2srcml --tabs '2' --position sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml -l C++ --tabs '2' --position -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() sxmlfile)
src2srcml --tabs '2' --position sub/a.cpp -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --tabs --position 14)
src2srcml -l C++ --tabs 'a' --position 14)

# register language

define output <<- 'STDOUT'
	sxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="Java" filename=sub/a.cpp/>
	STDOUT
INPUT
f = open(sub/a.cpp 'w')
f.write
f.close()

src2srcml --register-ext 'cpp=Java' sub/a.cpp "" fsxmlfile)
src2srcml --register-ext + '=cpp=Java' sub/a.cpp "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml --register-ext 'cpp=Java' sub/a.cpp -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --register-ext 1)
src2srcml -l C++ --register-ext "cpp=Jawa" )

define output <<- 'STDOUT'
	fsxmlfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="Java" filename="sub/a.xml"/>
	STDOUT
INPUT
if platform.system() != "Windows":
	os.system("touch su
b/a.xml")
else :
	os.system("copy emptysrc\\empty.cpp sub\\a.xml")

src2srcml --register-ext 'xml=Java' 'sub/a.xml' "" fsxmlfile)
src2srcml --register-ext + '=xml=Java' 'sub/a.xml' "" fsxmlfile)
if sys.platform != 'cygwin' :
	src2srcml --register-ext 'xml=Java' 'sub/a.xml' -o sub/a.cpp.xml
	readfile input sub/a.cpp.xml').read() fsxmlfile)


src2srcml -l C++ --register-ext "xml=Jawa" 1)

##
# directory input

# test
define output <<- 'STDOUT'
	srcmlstart = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	STDOUT
INPUT
aj = STDOUT
<unit language="AspectJ" filename="dir/file.aj">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

c = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="dir/file.c">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

cpp = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="dir/file.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

java = STDOUT
<unit language="Java" filename="dir/file.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

# test
srcmlend = STDOUT
</unit>
	STDOUT

if platform.system() != "Windows" :
	dir = execute(['ls' 'dir'.split("\n")
else :
	dir = os.listdir('dir')

# test
srcml = srcmlstart

for file in dir :
	if file == 'file.aj' :
	srcml += aj
	if file == 'file.c' :
	srcml += c
	if file == 'file.cpp' :
	srcml += cpp
	if file == 'file.java' :
	srcml += java

# test
srcml += srcmlend

if platform.system() == "Windows" or sys.platform == 'cygwin' :
	srcml = string.replace(srcml "dir/" "dir\\")

echo -n "" | src2srcml dir
src2srcml dir' -o 'dir/dir.xml' ""
readfile input dir/dir.xml' 'r').read() srcml)

if platform.system() != "Windows" and sys.platform != 'cygwin' :
	execute(['tar' 'czf' 'dir/foo.tar' 'dir/file.c'

echo -n "" |	 src2srcml dir
	src2srcml dir' -o 'dir/dir.xml' ""
	readfile input dir/dir.xml' 'r').read() srcml)

	execute(['rm' 'dir/foo.tar'

#
# nested files

src = STDOUT
a;
	STDOUT

# test
define output <<- 'STDOUT'
	srcmlstart = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	STDOUT
INPUT
cpp = STOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

cppempty= STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp/>
	STDOUT

java = STDOUT
<unit language="Java" filename="sub/a.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOU

javaempty = STDOUT
<unit language="Java" filename="sub/a.java"/>
	STDOUT

# test
srcmlend = STDOT
</unit>
	STDOUT

if platform.system() != "Windows" :
	os.system('rm sub/a.cpp; touch sub/a.cpp')
else :
	os.system("del sub\\a.cpp")
	os.system("copy emptysrc\\empty.cpp sub\\a.cpp")

f = open('sub/a.java' 'w')
f.write(src)
f.close()

src2srcml sub/a.cpp' 'sub/a.java' '' srcmlstart + cppempty + java + srcmlend)
src2srcml sub/a.cpp' 'sub/a.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cppempty + java + srcmlend)

src2srcml sub/a.java' sub/a.cpp '' srcmlstart + java + cppempty + srcmlend)
src2srcml sub/a.java' sub/a.cpp -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + java + cppempty + srcmlend)

if platform.system() != "Windows" :
	os.system('rm sub/a.java; touch sub/a.java')
else :
	os.system("del sub\\a.java")
	os.system("copy emptysrc\\empty.java sub\\a.java")

f = open(sub/a.cpp 'w')
f.write(src)
f.close()

src2srcml sub/a.cpp' 'sub/a.java' '' srcmlstart + cpp + javaempty + srcmlend)
src2srcml sub/a.cpp' 'sub/a.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cpp + javaempty + srcmlend)

src2srcml sub/a.java' sub/a.cpp '' srcmlstart + javaempty + cpp + srcmlend)
src2srcml sub/a.java' sub/a.cpp -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + javaempty + cpp + srcmlend)

cpp = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

if platform.system() != "Windows" :
	os.system('rm sub/a.cpp; touch sub/a.cpp')
else :
	os.system("del sub\\a.cpp")
	os.system("copy emptysrc\\empty.cpp sub\\a.cpp")

f = open('sub/b.cpp' 'w')
f.write(src)
f.close()

src2srcml sub/a.cpp' 'sub/b.cpp' '' srcmlstart + cppempty + cpp + srcmlend)
src2srcml sub/a.cpp' 'sub/b.cpp' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cppempty + cpp + srcmlend)

src2srcml sub/b.cpp' sub/a.cpp '' srcmlstart + cpp + cppempty + srcmlend)
src2srcml sub/b.cpp' sub/a.cpp -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cpp + cppempty + srcmlend)

java = STDOUT
<unit language="Java" filename="sub/b.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

if platform.system() != "Windows" :
	os.system('rm sub/a.java; touch sub/a.java')
else :
	os.system("del sub\\a.java")
	os.system("copy emptysrc\\empty.java sub\\a.java")

f = open('sub/b.java' 'w')
f.write(src)
f.close()

src2srcml sub/a.java' 'sub/b.java' '' srcmlstart + javaempty + java + srcmlend)
src2srcml sub/a.java' 'sub/b.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + javaempty + java + srcmlend)

src2srcml sub/b.java' 'sub/a.java' '' srcmlstart + java + javaempty + srcmlend)
src2srcml sub/b.java' 'sub/a.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + java + javaempty + srcmlend)

# xml error test
info_single = STDOUTxmlns="http://www.sdml.info/srcML/src"
xmlns:cpp="http://www.sdml.info/srcML/cpp"
encoding="UTF-8"
language="C++"
	STDOUT

info_archive = STDOUTxmlns="http://www.sdml.info/srcML/src"
encoding="UTF-8"
	STDOUT
# ok
srcml2src --info 'xml_error/illformed.xml' '' info_single)
srcml2src --info 'xml_error/illformedarchive.xml' '' info_archive)

xml_error = STDOUTsrcml2src: expected '>' in 'xml_error/illformed.xml'
	STDOUT

xml_archive_error = STDOUTsrcml2src: Extra content at the end of the document in 'xml_error/illformedarchive.xml'
	STDOUT

# bad
checkError([srcml2src 'xml_error/illformed.xml' '' xml_error)
checkError([srcml2src --units 'xml_error/illformedarchive.xml' '' xml_archive_error)

# escaped xml test

extract_option_xpath_simple ="concat('&lt;' string(//src:decl[string(src:name)='TO_DIR_FLAG'/src:name) '&gt;')"

extract_option_xpath = "concat('&lt;&#33;ENTITY ' string(//src:decl[substring(src:name string-length(src:name) - 4)='_FLAG'/src:name) ' \"--' substring(string(//src:decl[substring(src:name string-length(src:name) - 4)='_FLAG'/src:init/src:expr) 2 string-length(string(//src:decl[substring(src:name string-length(src:name) - 4)='_FLAG'/src:init/src:expr)) - 2) '\"&gt;')"

extract_option_xpath_output_simple = STDOUT<TO_DIR_FLAG>
	STDOUT

extract_option_xpath_output = STDOUT<!ENTITY TO_DIR_FLAG "--to-dir">
	STDOUT

extract_options_output = STDOUT<!ENTITY TO_DIR_FLAG "--to-dir">
<!ENTITY TO_DIR_FLAG_SHORT '-a'>
<!ENTITY UNIT_FLAG "--unit">
<!ENTITY UNIT_FLAG_SHORT '-U'>
<!ENTITY XML_FLAG "--xml">
<!ENTITY XML_FLAG_SHORT '-X'>
<!ENTITY INFO_FLAG "--info">
<!ENTITY INFO_FLAG_SHORT '-i'>
<!ENTITY LONG_INFO_FLAG "--longinfo">
<!ENTITY LONG_INFO_FLAG_SHORT '-L'>
<!ENTITY NAMESPACE_FLAG "--prefix">
<!ENTITY NAMESPACE_FLAG_SHORT '-p'>
<!ENTITY OMIT_FLAG "--omit">
<!ENTITY OMIT_FLAG_SHORT -o>
<!ENTITY XPATH_FLAG "--xpath">
<!ENTITY XSLT_FLAG "--xslt">
<!ENTITY PARAM_FLAG "--xpathparam">
<!ENTITY STRING_PARAM_FLAG "--param">
<!ENTITY APPLY_ROOT_FLAG "--apply-root">
<!ENTITY RELAXNG_FLAG "--relaxng">
<!ENTITY CONTEXT_FLAG "--context">
<!ENTITY LIST_FLAG "--list">
<!ENTITY REGISTER_EXTENSION_FLAG "--register-ext">
<!ENTITY REGISTER_EXTENSION_FILE_FLAG "--register-ext-file">
<!ENTITY REGISTER_EXTENSION_FUNCTION_FLAG "--register-xpath-func">
<!ENTITY REGISTER_EXTENSION_FUNCTION_FILE_FLAG "--register-xpath-func-file">
<!ENTITY EOL_FLAG "--eol">
	STDOUT

srcml2src --xpath extract_option_xpath_simple 'extract_options/extract_options_test.cpp.xml' '' extract_option_xpath_output_simple)
srcml2src --xpath extract_option_xpath 'extract_options/extract_options_test.cpp.xml' '' extract_option_xpath_output)
srcml2src --xslt 'extract_options/extract_options.xsl' 'extract_options/extract_options_test.cpp.xml' '' extract_options_output)

# UTF-8 BOM

src_no_bom = STDOUTa;
	STDOUT
src_bom = STDOUT\xef\xbb\xbfa;
	STDOUT

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
		STDOUT

src2srcml --language 'C' src_no_bom srcml)
src2srcml --language 'C' src_bom srcml)

# xpath various return types

# attribute

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
		STDOUT

# test
define output <<- 'STDOUT'
	srcml_nested = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
	STDOUT

xpath_attribute = "//src:unit/@filename"

xpath_attribute_string = "string(//src:unit/@filename)"

define output <<- 'STDOUT'
	xpath_attribute_output = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT

</unit>
	STDOUT

xpath_attribute_string_output = STDOUTa.cpp
	STDOUT

define output <<- 'STDOUT'
	xpath_attribute_nested_output = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp" item="1">b.cpp</unit>

</unit>
	STDOUT

xpath_attribute_string_nested_output = STDOUTa.cpp
b.cpp
	STDOUT

srcml2src --xpath xpath_attribute srcml xpath_attribute_output)
srcml2src --xpath xpath_attribute_string srcml xpath_attribute_string_output)

srcml2src --xpath xpath_attribute srcml_nested xpath_attribute_nested_output)
srcml2src --xpath xpath_attribute_string srcml_nested xpath_attribute_string_nested_output)

# comment

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	INPUT
</unit>
	STDOUT

# test
define output <<- 'STDOUT'
	srcml_nested = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT
<!-- Comment One -->
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
<!-- Comment Two -->
</unit>

</unit>
	STDOUT

xpath_comment = "//comment()"

xpath_comment_string = "string(//comment())"

define output <<- 'STDOUT'
	xpath_comment_output = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT

</unit>
	STDOUT

xpath_comment_string_output = STDOUT Comment 
	STDOUT

define output <<- 'STDOUT'
	xpath_comment_nested_output = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp" item="1"><!-- Comment Two --></unit>

</unit>
	STDOUT

xpath_comment_string_nested_output = STDOUT Comment One 
 Comment Two 
	STDOUT

srcml2src --xpath xpath_comment srcml xpath_comment_output)
srcml2src --xpath xpath_comment_string srcml xpath_comment_string_output)

srcml2src --xpath xpath_comment srcml_nested xpath_comment_nested_output)
srcml2src --xpath xpath_comment_string srcml_nested xpath_comment_string_nested_output)

# check srcml2src diff option

f = open('diff/diff.cpp.xml' 'r')
srcdiff = f.read()
f.close()

f = open('diff/a.cpp' 'r')
src_old = f.read()
f.close()

f = open('diff/a.cpp.xml' 'r')
# test
srcml_old = f.read()
f.close()

f = open('diff/b.cpp' 'r')
src_new = f.read()
f.close()

f = open('diff/b.cpp.xml' 'r')
# test
srcml_new = f.read()
f.close()

if False :
	srcml2src --revision '1' srcdiff src_old)
	srcml2src --revision '1' 'diff/diff.cpp.xml' '' src_old)
	srcml2src --revision '2' srcdiff src_new)
	srcml2src --revision '2' 'diff/diff.cpp.xml' '' src_new)
	
	srcml2src --revision '1' -o 'diff/old.cpp' srcdiff '')
	readfile input diff/old.cpp' 'r').read() src_old)
	srcml2src --revision '1' 'diff/diff.cpp.xml' -o 'diff/old.cpp' '' '')
	readfile input diff/old.cpp' 'r').read() src_old)
	srcml2src --revision '2' -o 'diff/new.cpp' srcdiff '')
	readfile input diff/new.cpp' 'r').read() src_new)
	srcml2src --revision '2' 'diff/diff.cpp.xml' -o 'diff/new.cpp' '' '')
	readfile input diff/new.cpp' 'r').read() src_new)

	srcml2src --xml --revision '1' 'diff/diff.cpp.xml' '' srcml_old)
	srcml2src --xml --revision '2' 'diff/diff.cpp.xml' '' srcml_new)

	srcml2src --xml --revision '1' -o 'diff/old.cpp.xml' srcdiff '')
	readfile input diff/old.cpp.xml' 'r').read() srcml_old)
	srcml2src --xml --revision '1' 'diff/diff.cpp.xml' -o 'diff/old.cpp.xml' '' '')
	readfile input diff/old.cpp.xml' 'r').read() srcml_old)
	srcml2src --xml --revision '2' -o 'diff/new.cpp.xml' srcdiff '')
	readfile input diff/new.cpp.xml' 'r').read() srcml_new)
	srcml2src --xml --revision '2' 'diff/diff.cpp.xml' -o 'diff/new.cpp.xml' '' '')
	readfile input diff/new.cpp.xml' 'r').read() srcml_new)

# cpp option

sfile1 = ""


f = open('sub/a.java' 'w')
f.write(sfile1)
f.close()

# test
##
# empty with debug
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java"/>
	STDOUT
INPUT
define output <<- 'STDOUT'
	f<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" filename="sub/a.java"/>
	INPUT
src2srcml sub/a.java' --cpp fsrcml)
src2srcml sub/a.java' --cpp fsrcml)
src2srcml sub/a.java' --cpp fsrcml)
if sys.platform != 'cygwin' :
	src2srcml -l 'Java' --cpp -o 'sub/a.java.xml' sfile1
	readfile input sub/a.java.xml' 'r').read() srcml)
src2srcml --cpp 'sub/a.java'-o 'sub/a.java.xml' ""
readfile input sub/a.java.xml' 'r').read() fsrcml)

# test
# footer
print
print "Error count:\t\t" error_count "\t" error_list
print "EOL Error count:\t" eol_error_count "\t" eol_error_list
print "Exception count:\t" exception_count
print

for i in range(len(error_list)) :
	print str(error_list[i) + "\t" + error_lines[i
print
print src2srcmlversion()
print srcml2srcversion()

cli_file = open("srcMLcliTestReport.txt" "w")
cli_file.write("Error count:\t\t" +  str(error_count) + "\t" + str(error_list) + "\n")
cli_file.write("EOL Error count:\t" +  str(eol_error_count) + "\t" + str(eol_error_list) + "\n")
cli_file.write("Exception count:\t" +  str(exception_count) + "\n")
cli_file.close()

exit
