#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
srcmlend = STDOT
</unit>
	STDOUT

if platform.system() != "Windows" :
	os.system('rm sub/a.cpp; touch sub/a.cpp)
else :
	os.system("del sub\\a.cpp")
	os.system("copy emptysrc\\empty.cpp sub\\a.cpp")

f = open('sub/a.java' 'w')
f.write(src)
f.close()

src2srcml sub/a.cpp 'sub/a.java' '' srcmlstart + cppempty + java + srcmlend)
src2srcml sub/a.cpp 'sub/a.java' -o 'sub/all.xml' '' '')
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

src2srcml sub/a.cpp 'sub/a.java' '' srcmlstart + cpp + javaempty + srcmlend)
src2srcml sub/a.cpp 'sub/a.java' -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + cpp + javaempty + srcmlend)

src2srcml sub/a.java' sub/a.cpp '' srcmlstart + javaempty + cpp + srcmlend)
src2srcml sub/a.java' sub/a.cpp -o 'sub/all.xml' '' '')
readfile input sub/all.xml' 'r').read() srcmlstart + javaempty + cpp + srcmlend)

cpp = STDOUT
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/b.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
	STDOUT

if platform.system() != "Windows" :
	os.system('rm sub/a.cpp; touch sub/a.cpp)
else :
	os.system("del sub\\a.cpp")
	os.system("copy emptysrc\\empty.cpp sub\\a.cpp")

f = open('sub/b.cpp' 'w')
f.write(src)
f.close()

src2srcml sub/a.cpp 'sub/b.cpp' '' srcmlstart + cppempty + cpp + srcmlend)
src2srcml sub/a.cpp 'sub/b.cpp' -o 'sub/all.xml' '' '')
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

