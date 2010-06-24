#!/usr/bin/python
#
# clitest.py
#
# Michael L. Collard
# collard@cs.kent.edu

import os.path
import subprocess

import option
import status

error_count = 0;

def check(command, input, output):

	print os.path.basename(command[0]), ' '.join(command[1:])
	
	line = execute(command, input)

	return validate(output, line)
	#return validate(output.strip(), line.strip())

def checkError(command, input, error) :
	print os.path.basename(command[0]), ' '.join(command[1:])

	line = executeWithError(command, input)

	return validate(error, line)
	
def validate(org, gen):
	if org != gen:
		globals()["error_count"] = globals()["error_count"] + 1
		print "ERROR"
		print "org|" + str(org) + "|"
		print "gen|" + str(gen) + "|"
	return

def execute(command, input):
	p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	last_line = p.communicate(input)[0]

	if p.returncode != 0:
		globals()["error_count"] = globals()["error_count"] + 1
		print "Status error:  ", p.returncode, command

	return last_line

def executeWithError(command, input):
	p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	last_line = p.communicate(input)[0]

	if p.returncode == 0:
		globals()["error_count"] = globals()["error_count"] + 1
		print "Status error:  ", p.returncode, command

	return last_line

def getreturn(command, input):
	p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	p.communicate(input)
	print "Status: ", p.returncode, "\t", os.path.basename(command[0]), ' '.join(command[1:])
	return p.returncode

def checkallforms(base, shortflag, longflag, optionvalue, progin, progout):
	if optionvalue != "":
		check([base, shortflag, optionvalue], progin, progout)
		check([base, longflag, optionvalue], progin, progout)
		check([base, longflag + "=" + optionvalue], progin, progout)
	else:
		check([base, shortflag], progin, progout)
		check([base, longflag], progin, progout)

	return
	
def checkallformsext(base, shortflag, longflag, optionvalue, other, progin, progout):
	if optionvalue != "":
		check([base, shortflag, optionvalue, other], progin, progout)
		check([base, longflag, optionvalue, other], progin, progout)
		check([base, longflag + "=" + optionvalue, other], progin, progout)
	else:
		check([base, shortflag, other], progin, progout)
		check([base, longflag, other], progin, progout)

	return
	
# version of src2srcml
def src2srcmlversion():

	last_line = subprocess.Popen([srcmltranslator, "-V"],
				     stdout=subprocess.PIPE).communicate()[0]

	return last_line.strip()

# version of srcml2src
def srcml2srcversion():

	last_line = subprocess.Popen([srcmlutility, "-V"],
				     stdout=subprocess.PIPE).communicate()[0]

	return last_line.strip()

srcmltranslator = os.environ.get("SRC2SRCML")
if srcmltranslator == "" or srcmltranslator == None:
	srcmltranslator = "../../bin/src2srcml"
	
srcmlutility = os.environ.get("SRCML2SRC")
if srcmlutility == "" or srcmlutility == None:
	srcmlutility = "../../bin/srcml2src"

src2srcml_src_encoding = src2srcmlversion().find("Libxml2") != -1;
srcml2src_src_encoding = srcml2srcversion().find("Libxml2") != -1;

if src2srcml_src_encoding:
	default_src2srcml_encoding = "UTF-8"
else:
	default_src2srcml_encoding = "ISO-8859-1"

if srcml2src_src_encoding:
	default_srcml2src_encoding = "UTF-8"
else:
	default_srcml2src_encoding = "ISO-8859-1"

xml_declaration= '<?xml version="1.0" encoding="' + default_src2srcml_encoding + '" standalone="yes"?>'

print xml_declaration

bad_encoding = "ISO"
bad_language = "Python++"

##
# empty default
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
check([srcmltranslator], "", srcml)

##
# empty with debug
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
"""
check([srcmltranslator, "--debug"], "", srcml)

##
# language flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
checkallforms(srcmltranslator, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C++", "", srcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
"""
checkallforms(srcmltranslator, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C", "", srcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="Java"/>
"""
checkallforms(srcmltranslator, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "Java", "", srcml)

##
# filename flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo"/>
"""
checkallforms(srcmltranslator, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "foo", "", srcml)

# filenames are not expanded if specified (unlike when extracted from name)
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="bar/foo"/>
"""
checkallforms(srcmltranslator, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "bar/foo", "", srcml)

# filename and directory specified
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo"/>
"""
check([srcmltranslator, option.FILENAME_FLAG_SHORT, "foo", option.DIRECTORY_FLAG_SHORT, "bar"], "", srcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo"/>
"""
check([srcmltranslator, option.DIRECTORY_FLAG_SHORT, "bar", option.FILENAME_FLAG_SHORT, "foo"], "", srcml)

##
# directory flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar"/>
"""
checkallforms(srcmltranslator, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "bar", "", srcml)

##
# version flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" version="1.0"/>
"""
checkallforms(srcmltranslator, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "1.0", "", srcml)

##
# xml encoding flag
srcml = """<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
checkallforms(srcmltranslator, option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "ISO-8859-1", "", srcml)

##
# create testing files
#os.system("mkdir sub");

sfile1 = """
a;
"""

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""
check([srcmltranslator], sfile1, sxmlfile1)

sfile2 = """
b;
"""

sxmlfile2 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>
"""
check([srcmltranslator], sfile2, sxmlfile2)

check([srcmltranslator, "-", "sub/a.cpp.xml"], sfile1, "")

check([srcmltranslator, "-", "sub/b.cpp.xml"], sfile2, "")

nestedfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">

<unit language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">

<unit language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfilesrc = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">

<src:unit language="C++" dir="sub" filename="a.cpp">
<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
</src:unit>

<src:unit language="C++" dir="sub" filename="b.cpp">
<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
</src:unit>

</src:unit>
"""

if not os.path.exists("sub"):
	os.mkdir("sub")
f = open("sub/a.cpp", "w")
f.write("\na;\n")
f.close()

f = open("sub/b.cpp", "w")
f.write("\nb;\n")
f.close()

check([srcmltranslator, "sub/a.cpp", "sub/b.cpp", "-"], "", nestedfile)

check([srcmltranslator, option.NESTED_FLAG, "sub/a.cpp", "-"], "", nestedfile1)

filelist = """
sub/a.cpp
# fff
sub/b.cpp
"""

f = open('filelistab', 'w')
f.write("\nsub/a.cpp\nsub/b.cpp\n\n")
f.close()

check([srcmltranslator, option.FILELIST_FLAG, "filelistab"], "", nestedfile)


####
# srcml2src

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo" version="1.2"/>
"""

checkallforms(srcmlutility, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", srcml, "C++\n")
checkallforms(srcmlutility, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", srcml, "bar\n")
checkallforms(srcmlutility, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", srcml, "foo\n")
checkallforms(srcmlutility, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", srcml, "1.2\n")
checkallforms(srcmlutility, option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "", srcml, default_srcml2src_encoding + "\n")

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="" dir="" filename="" version=""/>
"""

checkallforms(srcmlutility, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", srcml, "\n")
checkallforms(srcmlutility, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", srcml, "\n")
checkallforms(srcmlutility, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", srcml, "\n")
checkallforms(srcmlutility, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", srcml, "\n")

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
"""

checkallforms(srcmlutility, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", srcml, "")
checkallforms(srcmlutility, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", srcml, "")
checkallforms(srcmlutility, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", srcml, "")
checkallforms(srcmlutility, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", srcml, "")

check([srcmlutility, option.NESTED_FLAG], srcml, "0\n")
check([srcmlutility, option.NESTED_FLAG], nestedfile, "2\n")

check([srcmlutility, option.NESTED_FLAG], nestedfilesrc, "2\n")

checkallforms(srcmlutility, "-U", option.UNIT_FLAG, "1", nestedfile, sfile1)
check([srcmlutility, option.UNIT_FLAG, "1", "-"], nestedfile, sfile1)

checkallforms(srcmlutility, "-U", option.UNIT_FLAG, "2", nestedfile, sfile2)
check([srcmlutility, option.UNIT_FLAG, "2"], nestedfile, sfile2)

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", "-"], nestedfile, sxmlfile1)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1"], nestedfile, sxmlfile1)

sxmlfile2 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>
"""
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", "-"], nestedfile, sxmlfile2)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2"], nestedfile, sxmlfile2)

os.system("rm -f sub/a.cpp sub/b.cpp")

check([srcmlutility, option.EXPAND_FLAG], nestedfile, "")

validate(open("sub/a.cpp", "r").read(), sfile1)
validate(open("sub/b.cpp", "r").read(), sfile2)

os.system("rm -f sub/a.cpp sub/b.cpp")

check([srcmlutility, option.EXPAND_FLAG_SHORT], nestedfile, "")

validate(open("sub/a.cpp", "r").read(), sfile1)
validate(open("sub/b.cpp", "r").read(), sfile2)

# srcml2src extract nested unit
nestedfileextra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">

<unit language="C" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit language="Java" dir="sub" mytag="foo" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

sxmlfile1extra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", "-"], nestedfileextra, sxmlfile1extra)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1"], nestedfileextra, sxmlfile1extra)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.FILENAME_FLAG], nestedfileextra, "a.cpp\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.FILENAME_FLAG], nestedfileextra, "b.cpp\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.DIRECTORY_FLAG], nestedfileextra, "sub\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.DIRECTORY_FLAG], nestedfileextra, "sub\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.LANGUAGE_FLAG], nestedfileextra, "C\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.LANGUAGE_FLAG], nestedfileextra, "Java\n")

# srcml2src extract nested unit
nestedfileextra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lc="http://www.sdml.info/srcML/linecol" language="C++">

<unit language="C" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit language="Java" dir="sub" mytag="foo" filename="b.cpp">
<expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
</unit>

<unit dir="sub" mytag="foo" filename="c.cpp">
<expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
</unit>

</unit>
"""

sxmlfile1extra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lc="http://www.sdml.info/srcML/linecol" language="C" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

sxmlfile2extra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lc="http://www.sdml.info/srcML/linecol" language="Java" dir="sub" mytag="foo" filename="b.cpp">
<expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
</unit>
"""

sxmlfile3extra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lc="http://www.sdml.info/srcML/linecol" language="C++" dir="sub" mytag="foo" filename="c.cpp">
<expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
</unit>
"""

check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", "-"], nestedfileextra, sxmlfile1extra)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1"], nestedfileextra, sxmlfile1extra)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", "-"], nestedfileextra, sxmlfile2extra)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2"], nestedfileextra, sxmlfile2extra)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "3", "-"], nestedfileextra, sxmlfile3extra)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "3"], nestedfileextra, sxmlfile3extra)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.FILENAME_FLAG], nestedfileextra, "a.cpp\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.FILENAME_FLAG], nestedfileextra, "b.cpp\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.DIRECTORY_FLAG], nestedfileextra, "sub\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.DIRECTORY_FLAG], nestedfileextra, "sub\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.LANGUAGE_FLAG], nestedfileextra, "C\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.LANGUAGE_FLAG], nestedfileextra, "Java\n")

##
# src2srcml error return

# invalid input filename
validate(getreturn([srcmltranslator, "foobar"], None), status.STATUS_INPUTFILE_PROBLEM)

# invalid input filename (repeat in output)
validate(getreturn([srcmltranslator, "sub/a.cpp", "sub/a.cpp"], None), status.STATUS_INPUTFILE_PROBLEM)

# unknown option
validate(getreturn([srcmltranslator, "--strip", "foobar"], None), status.STATUS_UNKNOWN_OPTION)

# unknown encoding

if src2srcml_src_encoding:
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG + "=" + bad_encoding, "foobar"], None), status.STATUS_UNKNOWN_ENCODING)
	validate(getreturn([srcmltranslator, option.ENCODING_FLAG + "=" + bad_encoding, "foobar"], None), status.STATUS_UNKNOWN_ENCODING)
else:
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_LIBXML2_FEATURE)
	validate(getreturn([srcmltranslator, option.ENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_SUCCESS)
	
# missing value
validate(getreturn([srcmltranslator, option.LANGUAGE_FLAG, bad_language, "foobar"], None), status.STATUS_INVALID_LANGUAGE)
validate(getreturn([srcmltranslator, option.LANGUAGE_FLAG], None), status.STATUS_LANGUAGE_MISSING)
validate(getreturn([srcmltranslator, option.FILENAME_FLAG], ""), status.STATUS_FILENAME_MISSING)
validate(getreturn([srcmltranslator, option.DIRECTORY_FLAG], ""), status.STATUS_DIRECTORY_MISSING)
validate(getreturn([srcmltranslator, option.SRCVERSION_FLAG], ""), status.STATUS_VERSION_MISSING)

# source encoding not given
if src2srcml_src_encoding:
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG], ""), status.STATUS_SRCENCODING_MISSING)
else:
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG], ""), status.STATUS_LIBXML2_FEATURE)

validate(getreturn([srcmltranslator, option.ENCODING_FLAG], ""), status.STATUS_XMLENCODING_MISSING)

##
# srcml2src error return

# invalid input filename
validate(getreturn([srcmlutility, "foobar"], None), status.STATUS_INPUTFILE_PROBLEM)

# unknown option
validate(getreturn([srcmlutility, "--strip", "foobar"], None), status.STATUS_UNKNOWN_OPTION)

# unknown encoding
if srcml2src_src_encoding:
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG + "=" + bad_encoding], ""), status.STATUS_UNKNOWN_ENCODING)
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG], ""), status.STATUS_SRCENCODING_MISSING)
else:
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG + "=" + bad_encoding], ""), status.STATUS_LIBXML2_FEATURE)
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG, "foobar"], None), status.STATUS_LIBXML2_FEATURE)
	
# source encoding not given

# unit option selected but no value
validate(getreturn([srcmlutility, option.UNIT_FLAG], ""), status.STATUS_UNIT_MISSING)

# unit value too large
missing_unit = "3";
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit, option.XML_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit, option.FILENAME_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit, option.DIRECTORY_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit, option.SRCVERSION_FLAG], nestedfile), status.STATUS_UNIT_INVALID)

# invalid combinations
validate(getreturn([srcmlutility, option.XML_FLAG, option.TEXTENCODING_FLAG, "UTF-8", "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)

##
# cpp markup else

cpp_src = """
#if A
break;
#else
return;
#endif
"""

cpp_marked_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
<return>return;</return>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""
check([srcmltranslator], cpp_src, cpp_marked_srcml)
check([srcmltranslator, option.CPP_MARKUP_ELSE_FLAG], cpp_src, cpp_marked_srcml)


cpp_textonly_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
return;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""
check([srcmltranslator, option.CPP_TEXTONLY_ELSE_FLAG], cpp_src, cpp_textonly_srcml)

validate(getreturn([srcmltranslator, option.CPP_MARKUP_ELSE_FLAG, option.CPP_TEXTONLY_ELSE_FLAG, "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)
validate(getreturn([srcmltranslator, option.CPP_TEXTONLY_ELSE_FLAG, option.CPP_MARKUP_ELSE_FLAG, "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)

##
# cpp markup if0

cpp_if0 = """
#if 0
break;
#endif
"""

cpp_textonly_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
break;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""
check([srcmltranslator], cpp_if0, cpp_textonly_srcml)
check([srcmltranslator, option.CPP_TEXTONLY_IF0_FLAG], cpp_if0, cpp_textonly_srcml)

cpp_marked_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
<break>break;</break>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""
check([srcmltranslator, option.CPP_MARKUP_IF0_FLAG], cpp_if0, cpp_marked_srcml)

validate(getreturn([srcmltranslator, option.CPP_MARKUP_IF0_FLAG, option.CPP_TEXTONLY_IF0_FLAG, "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)
validate(getreturn([srcmltranslator, option.CPP_TEXTONLY_IF0_FLAG, option.CPP_MARKUP_IF0_FLAG, "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)

##
# xmlns options

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

# separate
check([srcmltranslator, "--xmlns=http://www.sdml.info/srcML/src"], "", srcml)
check([srcmltranslator, "--xmlns:cpp=http://www.sdml.info/srcML/cpp"], "", srcml)

# multiple
check([srcmltranslator, "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp"], "", srcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
"""

# separate
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr"], "", srcml)

# multiple
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp"], "", srcml)

check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:err=http://www.sdml.info/srcML/srcerr"], "", srcml)

check([srcmltranslator, "--debug", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr"], "", srcml)

srcml = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

srcml = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
check([srcmltranslator, "--xmlns:src=http://www.sdml.info/srcML/src", "--xmlns=http://www.sdml.info/srcML/cpp"], "", srcml)

##
# prefix extraction
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
"""

checkallforms(srcmlutility, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/src", srcml, """
""")

checkallforms(srcmlutility, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/cpp", srcml, """cpp
""")

checkallforms(srcmlutility, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/literal", srcml, "")

checkallforms(srcmlutility, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.ashland.edu/~mcollard/foo", srcml, "")

srcml = xml_declaration + """
<unit xmlns:cpp="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp"/>
"""

checkallforms(srcmlutility, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/src", srcml, """cpp
""")

checkallforms(srcmlutility, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/cpp", srcml, """
""")

##
# no xml declaration
srcml = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

check([srcmltranslator, option.NO_XML_DECLARATION_FLAG], "", srcml)

##
# no namespace declaration
srcml = xml_declaration + """
<unit language="C++"/>
"""

check([srcmltranslator, option.NO_NAMESPACE_DECLARATION_FLAG], "", srcml)

##
# check missingfile
srcml = srcmltranslator + " error: file 'foo.c' does not exist.\n"

checkError([srcmltranslator, 'foo.c'], "", srcml)

srcml = srcmltranslator + " error: file 'abc.c' does not exist.\n"

checkError([srcmltranslator, 'abc.c'], "", srcml)

srcml = srcmltranslator +  " error: file '../src/foo.c' does not exist.\n"

checkError([srcmltranslator, '../src/foo.c'], "", srcml)

##
# check correct language based on file extension

xmltag = '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'
opentag = '<unit '
namespaceone = 'xmlns="http://www.sdml.info/srcML/src" '
namespacetwo = 'xmlns:cpp="http://www.sdml.info/srcML/cpp" '
dir = 'dir="emptysrc" '
fileopen = 'filename="'
fileclose = '"'
endtag = '/>\n'

# check c file extensions
language = 'language="C" '

check([srcmltranslator, 'emptysrc/empty.c'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.c' + fileclose + endtag)
check([srcmltranslator, 'emptysrc/empty.h'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.h' + fileclose + endtag)

# check c++ file extensions
language = 'language="C++" '

check([srcmltranslator, 'emptysrc/empty.cpp'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.cpp' + fileclose + endtag)
check([srcmltranslator, 'emptysrc/empty.cc'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.cc' + fileclose + endtag)
check([srcmltranslator, 'emptysrc/empty.cxx'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.cxx' + fileclose + endtag)
check([srcmltranslator, 'emptysrc/empty.c++'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.c++' + fileclose + endtag)
check([srcmltranslator, 'emptysrc/empty.hpp'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.hpp' + fileclose + endtag)
check([srcmltranslator, 'emptysrc/empty.hh'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.hh' + fileclose + endtag)
check([srcmltranslator, 'emptysrc/empty.hxx'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.hxx' + fileclose + endtag)
check([srcmltranslator, 'emptysrc/empty.h++'], "", xmltag + opentag + namespaceone + namespacetwo + language + dir + fileopen + 'empty.h++' + fileclose + endtag)

# check java file extension
language = 'language="Java" '

check([srcmltranslator, 'emptysrc/empty.java'], "", xmltag + opentag + namespaceone + language + dir + fileopen + 'empty.java' + fileclose + endtag)

# check aspectj file extension
language = 'language="AspectJ" '

check([srcmltranslator, 'emptysrc/empty.aj'], "", xmltag + opentag + namespaceone + language + dir + fileopen + 'empty.aj' + fileclose + endtag)

# footer
print
print "Error count: ", error_count
print
print src2srcmlversion()
print srcml2srcversion()

exit
