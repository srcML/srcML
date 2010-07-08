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

def checkallformsfile(base, inputfile, shortflag, longflag, optionvalue, progin, progout):
	if optionvalue != "":
		check([base, inputfile, shortflag, optionvalue], progin, progout)
		check([base, inputfile, longflag, optionvalue], progin, progout)
		check([base, inputfile, longflag + "=" + optionvalue], progin, progout)
	else:
		check([base, inputfile, shortflag], progin, progout)
		check([base, inputfile, longflag], progin, progout)

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
checkallforms(srcmltranslator, option.DEBUG_FLAG_SHORT, option.DEBUG_FLAG, "", "", srcml)

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

check([srcmltranslator, "-", "-o", "sub/a.cpp.xml"], sfile1, "")

check([srcmltranslator, "-", "-o", "sub/b.cpp.xml"], sfile2, "")

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

check([srcmltranslator, "sub/a.cpp", "sub/b.cpp", "-o", "-"], "", nestedfile)

check([srcmltranslator, option.COMPOUND_FLAG, "sub/a.cpp", "-o", "-"], "", nestedfile1)

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
validate(getreturn([srcmltranslator, "sub/a.cpp", "-o", "sub/a.cpp"], None), status.STATUS_INPUTFILE_PROBLEM)

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

##
# Test output options

# src2srcml
sfile = """
a;
"""

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

fxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

check([srcmltranslator, 'sub/a.cpp', '--output', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fxmlfile)
check([srcmltranslator, 'sub/a.cpp', '--output=sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fxmlfile)
check([srcmltranslator, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fxmlfile)

check([srcmltranslator, '-', '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), sxmlfile)
check([srcmltranslator, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), sxmlfile)

check([srcmltranslator, '-', '--output', '/dev/stdout'], sfile, sxmlfile)
check([srcmltranslator, '-', '--output=/dev/stdout'], sfile, sxmlfile)
check([srcmltranslator, '-', '-o', '/dev/stdout'], sfile, sxmlfile)

# srcml2src

check([srcmlutility, 'sub/a.cpp.xml', '--output', 'sub/a.cpp'], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile)
check([srcmlutility, 'sub/a.cpp.xml', '--output=sub/a.cpp'], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile)
check([srcmlutility, 'sub/a.cpp.xml', '-o', 'sub/a.cpp'], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile)

check([srcmlutility, '-', '-o', 'sub/a.cpp'], sxmlfile, "")
validate(open('sub/a.cpp', 'r').read(), sfile)
check([srcmlutility, '-o', 'sub/a.cpp'], sxmlfile, "")
validate(open('sub/a.cpp', 'r').read(), sfile)

check([srcmlutility, '-', '--output', '/dev/stdout'], sxmlfile, sfile)
check([srcmlutility, '-', '--output=/dev/stdout'], sxmlfile, sfile)
check([srcmlutility, '-', '-o', '/dev/stdout'], sxmlfile, sfile)

##
# Test src2srcml options with files

sfile1 = ""

sfile2 = """
b;
"""

f = open('sub/a.cpp', 'w')
f.write(sfile1)
f.close()

##
# empty with debug
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++" dir="sub" filename="a.cpp"/>
"""
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.DEBUG_FLAG_SHORT, option.DEBUG_FLAG, "", "", fsrcml)
check([srcmltranslator, option.DEBUG_FLAG_SHORT, '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.DEBUG_FLAG_SHORT, 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# language flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp"/>
"""
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C++", "", fsrcml)
check([srcmltranslator, option.LANGUAGE_FLAG_SHORT, 'C++', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.LANGUAGE_FLAG_SHORT, 'C++', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" dir="sub" filename="a.cpp"/>
"""
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C", "", fsrcml)
check([srcmltranslator, option.LANGUAGE_FLAG_SHORT, 'C', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.LANGUAGE_FLAG_SHORT, 'C', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="Java"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="Java" dir="sub" filename="a.cpp"/>
"""
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "Java", "", fsrcml)
check([srcmltranslator, option.LANGUAGE_FLAG_SHORT, 'Java', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.LANGUAGE_FLAG_SHORT, 'Java', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# filename flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="foo"/>
"""
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "foo", "", fsrcml)
check([srcmltranslator, option.FILENAME_FLAG_SHORT, 'foo', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.FILENAME_FLAG_SHORT, 'foo', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# directory flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="a.cpp"/>
"""
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "bar", "", fsrcml)
check([srcmltranslator, option.DIRECTORY_FLAG_SHORT, 'bar', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.DIRECTORY_FLAG_SHORT, 'bar', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# version flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" version="1.0"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" version="1.0"/>
"""
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "1.0", "", fsrcml)
check([srcmltranslator, option.SRCVERSION_FLAG_SHORT, '1.0', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.SRCVERSION_FLAG_SHORT, '1.0', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# xml encoding flag
srcml = """<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = """<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp"/>
"""
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "ISO-8859-1", "", fsrcml)
check([srcmltranslator, option.ENCODING_FLAG_SHORT, 'ISO-8859-1', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.ENCODING_FLAG_SHORT, 'ISO-8859-1', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# text encoding flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp"/>
"""
checkallforms(srcmltranslator, option.TEXTENCODING_FLAG_SHORT, option.TEXTENCODING_FLAG, "ISO-8859-1", sfile1, srcml)
checkallformsfile(srcmltranslator, 'sub/a.cpp', option.TEXTENCODING_FLAG_SHORT, option.TEXTENCODING_FLAG, "ISO-8859-1", "", fsrcml)
check([srcmltranslator, option.TEXTENCODING_FLAG, "ISO-8859-1", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.TEXTENCODING_FLAG, "ISO-8859-1", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.TEXTENCODING_FLAG, "ISO-8859-1", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

# nested 

sfile1 = """
a;
"""

sfile2 = """
b;
"""


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

f = open('sub/a.cpp', 'w')
f.write(sfile1)
f.close()

f = open('sub/b.cpp', 'w')
f.write(sfile2)
f.close()

check([srcmltranslator, option.COMPOUND_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), nestedfile1)
check([srcmltranslator, 'sub/a.cpp', 'sub/b.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), nestedfile)
check([srcmltranslator, '--xmlns:src=http://www.sdml.info/srcML/src', 'sub/a.cpp', 'sub/b.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), nestedfilesrc)

# files from
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

filelist = """
sub/a.cpp
# fff
sub/b.cpp
"""

f = open('filelistab', 'w')
f.write("\nsub/a.cpp\nsub/b.cpp\n\n")
f.close()

check([srcmltranslator, option.FILELIST_FLAG, "filelistab", '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), nestedfile)

##
# xmlns options

sfile1 = ""

sfile2 = """
b;
"""

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp"/>
"""

f = open('sub/a.cpp', 'w')
f.write(sfile1)
f.close()

# separate
check([srcmltranslator, "--xmlns=http://www.sdml.info/srcML/src", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, "--xmlns=http://www.sdml.info/srcML/src", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, "--xmlns=http://www.sdml.info/srcML/src", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)
check([srcmltranslator, "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, "--xmlns:cpp=http://www.sdml.info/srcML/cpp", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

# multiple
check([srcmltranslator, "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++" dir="sub" filename="a.cpp"/>
"""

# separate
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

# multiple
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:err=http://www.sdml.info/srcML/srcerr", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, "--debug", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, "--debug", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, "--debug", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp"/>
"""
check([srcmltranslator, "--xmlns:src=http://www.sdml.info/srcML/src", "--xmlns=http://www.sdml.info/srcML/cpp", 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, "--xmlns:src=http://www.sdml.info/srcML/src", "--xmlns=http://www.sdml.info/srcML/cpp", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, "--xmlns:src=http://www.sdml.info/srcML/src", "--xmlns=http://www.sdml.info/srcML/cpp", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# no xml declaration
srcml = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.NO_XML_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# no namespace declaration
srcml = xml_declaration + """
<unit language="C++"/>
"""

fsrcml = xml_declaration + """
<unit language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.NO_NAMESPACE_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

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

fcpp_marked_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
<return>return;</return>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

f = open('sub/a.cpp', 'w')
f.write(cpp_src)
f.close()

check([srcmltranslator, 'sub/a.cpp'], "", fcpp_marked_srcml)
check([srcmltranslator, '-o', 'sub/a.cpp.xml'], cpp_src, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_marked_srcml)
check([srcmltranslator, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_marked_srcml)

check([srcmltranslator, option.CPP_MARKUP_ELSE_FLAG, 'sub/a.cpp'], "", fcpp_marked_srcml)
check([srcmltranslator, option.CPP_MARKUP_ELSE_FLAG, '-o', 'sub/a.cpp.xml'], cpp_src, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_marked_srcml)
check([srcmltranslator, option.CPP_MARKUP_ELSE_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_marked_srcml)

cpp_textonly_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
return;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

fcpp_textonly_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
return;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

check([srcmltranslator, option.CPP_TEXTONLY_ELSE_FLAG, 'sub/a.cpp'], "", fcpp_textonly_srcml)
check([srcmltranslator, option.CPP_TEXTONLY_ELSE_FLAG, '-o', 'sub/a.cpp.xml'], cpp_src, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_textonly_srcml)
check([srcmltranslator, option.CPP_TEXTONLY_ELSE_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_textonly_srcml)

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

fcpp_textonly_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
break;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

f = open('sub/a.cpp', 'w')
f.write(cpp_if0)
f.close()

check([srcmltranslator, 'sub/a.cpp'], "", fcpp_textonly_srcml)
check([srcmltranslator, '-o', 'sub/a.cpp.xml'], cpp_if0, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_textonly_srcml)
check([srcmltranslator, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_textonly_srcml)

check([srcmltranslator, option.CPP_TEXTONLY_IF0_FLAG, 'sub/a.cpp'], "", fcpp_textonly_srcml)
check([srcmltranslator, option.CPP_TEXTONLY_IF0_FLAG, '-o', 'sub/a.cpp.xml'], cpp_if0, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_textonly_srcml)
check([srcmltranslator, option.CPP_TEXTONLY_IF0_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_textonly_srcml)

cpp_marked_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
<break>break;</break>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

fcpp_marked_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
<break>break;</break>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

check([srcmltranslator, option.CPP_MARKUP_IF0_FLAG, 'sub/a.cpp'], "", fcpp_marked_srcml)
check([srcmltranslator, option.CPP_MARKUP_IF0_FLAG, '-o', 'sub/a.cpp.xml'], cpp_if0, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_marked_srcml)
check([srcmltranslator, option.CPP_MARKUP_IF0_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_marked_srcml)

##
# Test srcml2src options with files

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" version="1.2">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

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

f = open('sub/a.cpp.xml', 'w')
f.write(sxmlfile1)
f.close()
# check metadata options
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", "", "C++\n")
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", "", "sub\n")
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", "", "a.cpp\n")
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", "", "1.2\n")
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "", "", default_srcml2src_encoding + "\n")

check([srcmlutility, option.NESTED_FLAG, 'sub/a.cpp.xml'], "", "0\n")

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile1)
f.close()
check([srcmlutility, option.NESTED_FLAG, 'sub/a.cpp.xml'], "", "1\n")

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile)
f.close()
check([srcmlutility, option.NESTED_FLAG, 'sub/a.cpp.xml'], "", "2\n")

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfilesrc)
f.close()
check([srcmlutility, option.NESTED_FLAG, 'sub/a.cpp.xml'], "", "2\n")

# check unit option

sfile1 = """
a;
"""

sfile2 = """
b;
"""

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

sxmlfile2 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
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

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile)
f.close()

checkallformsfile(srcmlutility, 'sub/a.cpp.xml', "-U", option.UNIT_FLAG, "1", "", sfile1)
check([srcmlutility, option.UNIT_FLAG, "1", '-o', "sub/a.cpp"], nestedfile, "")
validate(open('sub/a.cpp', 'r').read(), sfile1)
check([srcmlutility, option.UNIT_FLAG, "1", 'sub/a.cpp.xml', '-o', "sub/a.cpp"], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile1)

checkallformsfile(srcmlutility, 'sub/a.cpp.xml', "-U", option.UNIT_FLAG, "2", "", sfile2)
check([srcmlutility, option.UNIT_FLAG, "2", '-o', "sub/b.cpp"], nestedfile, "")
validate(open('sub/b.cpp', 'r').read(), sfile2)
check([srcmlutility, option.UNIT_FLAG, "2", 'sub/a.cpp.xml', '-o', "sub/b.cpp"], "", "")
validate(open('sub/b.cpp', 'r').read(), sfile2)

# check xml and unit option

check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", 'sub/a.cpp.xml'], "", sxmlfile1)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", '-o', 'sub/b.cpp.xml'], nestedfile, "")
validate(open('sub/b.cpp.xml', 'r').read(), sxmlfile1)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", 'sub/a.cpp.xml', '-o', "sub/b.cpp.xml"], "", "")
validate(open('sub/b.cpp.xml', 'r').read(), sxmlfile1)

check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", 'sub/a.cpp.xml'], "", sxmlfile2)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", '-o', 'sub/b.cpp.xml'], nestedfile, "")
validate(open('sub/b.cpp.xml', 'r').read(), sxmlfile2)
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", 'sub/a.cpp.xml', '-o', "sub/b.cpp.xml"], "", "")
validate(open('sub/b.cpp.xml', 'r').read(), sxmlfile2)

# check metadata options with xml and unit

nestedfileextra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">

<unit language="C++" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit language="Java" dir="emptysrc" mytag="foo" filename="empty.java">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfileextra)
f.close()

check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.LANGUAGE_FLAG, 'sub/a.cpp.xml'], "", "C++\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.DIRECTORY_FLAG, 'sub/a.cpp.xml'], "", "sub\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "1", option.FILENAME_FLAG, 'sub/a.cpp.xml'], "", "a.cpp\n")

check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.LANGUAGE_FLAG, 'sub/a.cpp.xml'], "", "Java\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.DIRECTORY_FLAG, 'sub/a.cpp.xml'], "", "emptysrc\n")
check([srcmlutility, option.XML_FLAG, option.UNIT_FLAG, "2", option.FILENAME_FLAG, 'sub/a.cpp.xml'], "", "empty.java\n")

# prefix extraction

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(sxmlfile1)
f.close()
execute([srcmltranslator, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "")
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/src", "", """
""")

checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/cpp", "", """cpp
""")

checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/literal", "", "")

checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.ashland.edu/~mcollard/foo", "", "")

##
# text encoding flag

sfile1 = """
a;
"""

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

f = open('sub/a.cpp', 'w')
f.write(sfile1)
f.close()
checkallforms(srcmlutility, option.TEXTENCODING_FLAG_SHORT, option.TEXTENCODING_FLAG, "ISO-8859-1", sxmlfile1, sfile1)
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.TEXTENCODING_FLAG_SHORT, option.TEXTENCODING_FLAG, "ISO-8859-1", "", sfile1)
check([srcmlutility, option.TEXTENCODING_FLAG, "ISO-8859-1", 'sub/a.cpp.xml'], "", sfile1)
check([srcmlutility, option.TEXTENCODING_FLAG, "ISO-8859-1", '-o', 'sub/a.cpp'], sxmlfile1, "")
validate(open('sub/a.cpp', 'r').read(), sfile1)
check([srcmlutility, option.TEXTENCODING_FLAG, "ISO-8859-1", 'sub/a.cpp.xml', '-o', 'sub/a.cpp'], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile1)

##
# test compression tool

sfile = """
a;
"""

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

fxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

# src2srcml

f = open('sub/a.cpp', 'w')
f.write(sfile)
f.close()
check([srcmltranslator, option.COMPRESSED_FLAG_SHORT, 'sub/a.cpp', '-o', 'sub/a.cpp.xml.gz'], "", "")
check(['gunzip', '-c', 'sub/a.cpp.xml.gz'], "", fxmlfile)
check([srcmltranslator, option.COMPRESSED_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml.gz'], "", "")
check(['gunzip', '-c', 'sub/a.cpp.xml.gz'], "", fxmlfile)
check([srcmltranslator, option.COMPRESSED_FLAG_SHORT, '-o', 'sub/a.cpp.xml.gz'], sfile, "")
check(['gunzip', '-c', 'sub/a.cpp.xml.gz'], "", sxmlfile)


# srcml2src

f = open('sub/a.cpp.xml', 'w')
f.write(fxmlfile)
f.close()
check([srcmlutility, option.COMPRESSED_FLAG_SHORT, 'sub/a.cpp.xml', '-o', 'sub/a.cpp.gz'], "", "")
check(['gunzip', '-c', 'sub/a.cpp.gz'], "", sfile)
check([srcmlutility, option.COMPRESSED_FLAG, 'sub/a.cpp.xml', '-o', 'sub/a.cpp.gz'], "", "")
check(['gunzip', '-c', 'sub/a.cpp.gz'], "", sfile)
check([srcmlutility, option.COMPRESSED_FLAG_SHORT, '-o', 'sub/a.cpp.gz'], fxmlfile, "")
check(['gunzip', '-c', 'sub/a.cpp.gz'], "", sfile)

# test input file is gzipped
f = open('sub/a.cpp.xml.gz', 'r')
gzipped = f.read()
f.close()

check([srcmlutility], gzipped, sfile)
check([srcmlutility, 'sub/a.cpp.xml.gz'], "", sfile)
check([srcmlutility, '-o', 'sub/a.cpp'], gzipped, "")
validate(open('sub/a.cpp', 'r').read(), sfile)
check([srcmlutility, 'sub/a.cpp.xml.gz', '-o', 'sub/a.cpp'], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile)

##
# src2srcml Markup Extensions
sfile = ""

f = open('sub/a.cpp', 'w')
f.write(sfile)
f.close()

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.LITERAL_FLAG], sfile, srcml)
check([srcmltranslator, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.OPERATOR_FLAG], sfile, srcml)
check([srcmltranslator, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.MODIFIER_FLAG], sfile, srcml)
check([srcmltranslator, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.LITERAL_FLAG, option.OPERATOR_FLAG], sfile, srcml)
check([srcmltranslator, option.LITERAL_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.LITERAL_FLAG, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.LITERAL_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, option.OPERATOR_FLAG, option.LITERAL_FLAG], sfile, srcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.LITERAL_FLAG, option.MODIFIER_FLAG], sfile, srcml)
check([srcmltranslator, option.LITERAL_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.LITERAL_FLAG, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.LITERAL_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, option.MODIFIER_FLAG, option.LITERAL_FLAG], sfile, srcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.OPERATOR_FLAG, option.MODIFIER_FLAG], sfile, srcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, option.MODIFIER_FLAG, option.OPERATOR_FLAG], sfile, srcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" dir="sub" filename="a.cpp"/>
"""

check([srcmltranslator, option.LITERAL_FLAG, option.OPERATOR_FLAG, option.MODIFIER_FLAG], sfile, srcml)
check([srcmltranslator, option.LITERAL_FLAG, option.OPERATOR_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.LITERAL_FLAG, option.OPERATOR_FLAG, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.LITERAL_FLAG, option.OPERATOR_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, option.LITERAL_FLAG, option.MODIFIER_FLAG, option.OPERATOR_FLAG], sfile, srcml)
check([srcmltranslator, option.LITERAL_FLAG, option.MODIFIER_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.LITERAL_FLAG, option.MODIFIER_FLAG, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.LITERAL_FLAG, option.MODIFIER_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, option.OPERATOR_FLAG, option.LITERAL_FLAG, option.MODIFIER_FLAG], sfile, srcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.LITERAL_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.LITERAL_FLAG, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.LITERAL_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, option.OPERATOR_FLAG, option.MODIFIER_FLAG, option.LITERAL_FLAG], sfile, srcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.MODIFIER_FLAG, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.MODIFIER_FLAG, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.OPERATOR_FLAG, option.MODIFIER_FLAG, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, option.MODIFIER_FLAG, option.LITERAL_FLAG, option.OPERATOR_FLAG], sfile, srcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.LITERAL_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.LITERAL_FLAG, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.LITERAL_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([srcmltranslator, option.MODIFIER_FLAG, option.OPERATOR_FLAG, option.LITERAL_FLAG], sfile, srcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.OPERATOR_FLAG, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.OPERATOR_FLAG, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([srcmltranslator, option.MODIFIER_FLAG, option.OPERATOR_FLAG, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# srcml2src info and longinfo

info = """xmlns="http://www.sdml.info/srcML/src"
xmlns:cpp="http://www.sdml.info/srcML/cpp"
encoding="UTF-8"
language="C++"
directory="sub"
filename="a.cpp"
"""

longinfo = """xmlns="http://www.sdml.info/srcML/src"
xmlns:cpp="http://www.sdml.info/srcML/cpp"
encoding="UTF-8"
language="C++"
files="2"
"""

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
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

f = open('sub/a.cpp.xml', 'w')
f.write(sxmlfile1)
f.close()
checkallforms(srcmlutility, option.INFO_FLAG_SHORT, option.INFO_FLAG, "", sxmlfile, info)
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.INFO_FLAG_SHORT, option.INFO_FLAG, "", "", info)

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile)
f.close()
checkallforms(srcmlutility, option.LONG_INFO_FLAG_SHORT, option.LONG_INFO_FLAG, "", nestedfile, longinfo)
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.LONG_INFO_FLAG_SHORT, option.LONG_INFO_FLAG, "", "", longinfo)

##
# test extract all command

sfile1 = """
a;
"""

sfile2 = """
b;
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

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile)
f.close()
checkallforms(srcmlutility, option.EXPAND_FLAG_SHORT, option.EXPAND_FLAG, "", nestedfile, "")
validate(open('sub/a.cpp', 'r').read(), sfile1)
validate(open('sub/b.cpp', 'r').read(), sfile2)
checkallformsfile(srcmlutility, 'sub/a.cpp.xml', option.EXPAND_FLAG_SHORT, option.EXPAND_FLAG, "", "", "")
validate(open('sub/a.cpp', 'r').read(), sfile1)
validate(open('sub/b.cpp', 'r').read(), sfile2)

##
# no xml declaration srcml2src

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
</unit>"""

srcmlout = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

check([srcmlutility, option.NO_XML_DECLARATION_FLAG], srcml, srcmlout)
check([srcmlutility, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcmlutility, option.NO_XML_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcmlout)
check([srcmlutility, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

##
# no namespace declaration

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
</unit>"""

srcmlout = xml_declaration + """
<unit language="C++"/>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

check([srcmlutility, option.NO_NAMESPACE_DECLARATION_FLAG], srcml, srcmlout)
check([srcmlutility, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcmlutility, option.NO_NAMESPACE_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcmlout)
check([srcmlutility, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

##
# Help and version


# src2srcml
print os.path.basename(srcmltranslator) + ' ' + option.HELP_FLAG
line = execute([srcmltranslator, option.HELP_FLAG], "")
execute(['grep', 'Report bugs'], line)
print os.path.basename(srcmltranslator) + ' ' + option.VERSION_FLAG
line = execute([srcmltranslator, option.VERSION_FLAG], "")
execute(['grep', 'Copyright'], line)

# srcML2src
print os.path.basename(srcmlutility) + ' ' + option.HELP_FLAG
line = execute([srcmlutility, option.HELP_FLAG], "")
execute(['grep', 'Report bugs'], line)
print os.path.basename(srcmlutility) + ' ' + option.VERSION_FLAG
line = execute([srcmlutility, option.VERSION_FLAG], "")
execute(['grep', 'Copyright'], line)

##
# Test order of metadata option order

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" version="1.0"/>
"""

metadata = {option.LANGUAGE_FLAG_SHORT:'language="C++"\n', option.DIRECTORY_FLAG_SHORT:'directory="sub"\n', option.FILENAME_FLAG_SHORT:'filename="a.cpp"\n',
	    option.SRCVERSION_FLAG_SHORT:'src-version="1.0"\n', option.ENCODING_FLAG_SHORT:'encoding="UTF-8"\n'}
for metaoption in metadata.keys() :
	for othermeta in metadata.keys() :
		check([srcmlutility, metaoption + othermeta[1]], srcml, metadata[metaoption] + metadata[othermeta])

# footer
print
print "Error count: ", error_count
print
print src2srcmlversion()
print srcml2srcversion()

exit
