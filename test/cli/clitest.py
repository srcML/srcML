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

test_count = 0
error_count = 0

def check(command, input, output):

	globals()["test_count"] += 1
	print test_count, os.path.basename(command[0]), ' '.join(command[1:])
	
	line = execute(command, input)

	return validate(line, output)
	#return validate(output.strip(), line.strip())

def checkNoOutput(command, input):

	globals()["test_count"] += 1
	print test_count, os.path.basename(command[0]), ' '.join(command[1:])
	
	executeNoOutput(command, input)


def checkError(command, input, error) :

	globals()["test_count"] += 1
	print test_count, os.path.basename(command[0]), ' '.join(command[1:])

	line = executeWithError(command, input)

	return validate(line, error)
	
def validate(gen, expected):
	if gen != expected:
		globals()["error_count"] = globals()["error_count"] + 1
		print "ERROR"
		print "expected|" + str(expected) + "|"
		print "gen|" + str(gen) + "|"
	return

def execute(command, input):
	p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	last_line = p.communicate(input)[0]

	if p.returncode != 0:
		globals()["error_count"] = globals()["error_count"] + 1
		print "Status error:  ", p.returncode, command

	return last_line

def executeNoOutput(command, input) :
	p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=None, stderr=subprocess.PIPE)
	p.communicate(input)[0]

	if p.returncode != 0:
		globals()["error_count"] = globals()["error_count"] + 1
		print "Status error:  ", p.returncode, command
	
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
	globals()["test_count"] += 1
	print test_count, "Status: ", p.returncode, "\t", os.path.basename(command[0]), ' '.join(command[1:])
	return p.returncode

def checkallforms(base, shortflag, longflag, optionvalue, progin, progout):
	if base == src2srcml and (shortflag != option.LANGUAGE_FLAG_SHORT or longflag != option.LANGUAGE_FLAG_SHORT) :
		if optionvalue != "":
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', shortflag, optionvalue], progin, progout)
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag, optionvalue], progin, progout)
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag + "=" + optionvalue], progin, progout)
		else:
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', shortflag], progin, progout)
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag], progin, progout)

	else :
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

	last_line = subprocess.Popen([src2srcml, "-V"],
				     stdout=subprocess.PIPE).communicate()[0]

	return last_line.strip()

# version of srcml2src
def srcml2srcversion():

	last_line = subprocess.Popen([srcml2src, "-V"],
				     stdout=subprocess.PIPE).communicate()[0]

	return last_line.strip()

src2srcml = os.environ.get("SRC2SRCML")
if src2srcml == "" or src2srcml == None:
	src2srcml = "../../bin/src2srcml"
	
srcml2src = os.environ.get("SRCML2SRC")
if srcml2src == "" or srcml2src == None:
	srcml2src = "../../bin/srcml2src"

default_src2srcml_encoding = "UTF-8"
default_srcml2src_encoding = "UTF-8"

xml_declaration= '<?xml version="1.0" encoding="' + default_src2srcml_encoding + '" standalone="yes"?>'

print xml_declaration

bad_encoding = "ISO"
bad_language = "Python++"

##
# empty default
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++'], "", srcml)

##
# empty with debug
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
"""
checkallforms(src2srcml, option.DEBUG_FLAG_SHORT, option.DEBUG_FLAG, "", "", srcml)

##
# language flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
checkallforms(src2srcml, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C++", "", srcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
"""
checkallforms(src2srcml, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C", "", srcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="Java"/>
"""
checkallforms(src2srcml, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "Java", "", srcml)

##
# filename flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo"/>
"""
checkallforms(src2srcml, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "foo", "", srcml)

# filenames are not expanded if specified (unlike when extracted from name)
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="bar/foo"/>
"""
checkallforms(src2srcml, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "bar/foo", "", srcml)

# filename and directory specified
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo"/>
"""
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.FILENAME_FLAG_SHORT, "foo", option.DIRECTORY_FLAG_SHORT, "bar"], "", srcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo"/>
"""
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.DIRECTORY_FLAG_SHORT, "bar", option.FILENAME_FLAG_SHORT, "foo"], "", srcml)

##
# directory flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar"/>
"""
checkallforms(src2srcml, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "bar", "", srcml)

##
# version flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" version="1.0"/>
"""
checkallforms(src2srcml, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "1.0", "", srcml)

##
# xml encoding flag
srcml = """<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
checkallforms(src2srcml, option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "ISO-8859-1", "", srcml)

##
# create testing files
if not os.path.exists("sub"):
	os.system("mkdir sub")

sfile1 = """
a;
"""

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++'], sfile1, sxmlfile1)

sfile2 = """
b;
"""

sxmlfile2 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>
"""
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++'], sfile2, sxmlfile2)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "-", "-o", "sub/a.cpp.xml"], sfile1, "")

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "-", "-o", "sub/b.cpp.xml"], sfile2, "")

nestedfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfilesrc = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src">

<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
</src:unit>

<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
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

check([src2srcml, "sub/a.cpp", "sub/b.cpp", "-o", "-"], "", nestedfile)

check([src2srcml, option.COMPOUND_FLAG, "sub/a.cpp", "-o", "-"], "", nestedfile1)

filelist = """
sub/a.cpp
# fff
sub/b.cpp
"""

f = open('filelistab', 'w')
f.write("\nsub/a.cpp\nsub/b.cpp\n\n")
f.close()

check([src2srcml, option.FILELIST_FLAG, "filelistab"], "", nestedfile)


####
# srcml2src

src = """
a;
"""

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

check([srcml2src], srcml, src)
check([srcml2src, 'sub/a.cpp.xml'], "", src)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo" version="1.2"/>
"""

checkallforms(srcml2src, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", srcml, "C++\n")
checkallforms(srcml2src, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", srcml, "bar\n")
checkallforms(srcml2src, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", srcml, "foo\n")
checkallforms(srcml2src, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", srcml, "1.2\n")
checkallforms(srcml2src, option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "", srcml, default_srcml2src_encoding + "\n")

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="" dir="" filename="" version=""/>
"""

checkallforms(srcml2src, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", srcml, "\n")
checkallforms(srcml2src, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", srcml, "\n")
checkallforms(srcml2src, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", srcml, "\n")
checkallforms(srcml2src, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", srcml, "\n")

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
"""

checkallforms(srcml2src, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", srcml, "")
checkallforms(srcml2src, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", srcml, "")
checkallforms(srcml2src, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", srcml, "")
checkallforms(srcml2src, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", srcml, "")

check([srcml2src, option.NESTED_FLAG], srcml, "0\n")
check([srcml2src, option.NESTED_FLAG], nestedfile, "2\n")

check([srcml2src, option.NESTED_FLAG], nestedfilesrc, "2\n")

checkallforms(srcml2src, option.UNIT_FLAG_SHORT, option.UNIT_FLAG, "1", nestedfile, sfile1)
check([srcml2src, option.UNIT_FLAG, "1", "-"], nestedfile, sfile1)

checkallforms(srcml2src, option.UNIT_FLAG_SHORT, option.UNIT_FLAG, "2", nestedfile, sfile2)
check([srcml2src, option.UNIT_FLAG, "2"], nestedfile, sfile2)

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

nestedfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", "-"], nestedfile, sxmlfile1)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1"], nestedfile, sxmlfile1)

sxmlfile2 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>
"""
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", "-"], nestedfile, sxmlfile2)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2"], nestedfile, sxmlfile2)

os.system("rm -f sub/a.cpp")

checkNoOutput([srcml2src, option.EXPAND_FLAG + '=.'], sxmlfile1)

validate(open("sub/a.cpp", "r").read(), sfile1)

os.system("rm -f sub/a.cpp sub/b.cpp;")

checkNoOutput([srcml2src, option.EXPAND_FLAG + '=.'], nestedfile)

validate(open("sub/a.cpp", "r").read(), sfile1)
validate(open("sub/b.cpp", "r").read(), sfile2)

#os.system("rm -f sub/a.cpp sub/b.cpp")

checkNoOutput([srcml2src, option.EXPAND_FLAG_SHORT, '.'], nestedfile)

#validate(open("sub/a.cpp", "r").read(), sfile1)
#validate(open("sub/b.cpp", "r").read(), sfile2)

# srcml2src extract nested unit
nestedfileextra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="sub" mytag="foo" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

sxmlfile1extra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", "-"], nestedfileextra, sxmlfile1extra)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1"], nestedfileextra, sxmlfile1extra)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.FILENAME_FLAG], nestedfileextra, "a.cpp\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.FILENAME_FLAG], nestedfileextra, "b.cpp\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.DIRECTORY_FLAG], nestedfileextra, "sub\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.DIRECTORY_FLAG], nestedfileextra, "sub\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.LANGUAGE_FLAG], nestedfileextra, "C\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.LANGUAGE_FLAG], nestedfileextra, "Java\n")

# srcml2src extract nested unit
nestedfileextra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:lc="http://www.sdml.info/srcML/linecol" language="C++">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="sub" mytag="foo" filename="b.cpp">
<expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" dir="sub" mytag="foo" filename="c.cpp">
<expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
</unit>

</unit>
"""

sxmlfile1extra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:lc="http://www.sdml.info/srcML/linecol" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

sxmlfile2extra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:lc="http://www.sdml.info/srcML/linecol" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="sub" mytag="foo" filename="b.cpp">
<expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
</unit>
"""

sxmlfile3extra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:lc="http://www.sdml.info/srcML/linecol" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" mytag="foo" filename="c.cpp">
<expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
</unit>
"""

check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", "-"], nestedfileextra, sxmlfile1extra)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1"], nestedfileextra, sxmlfile1extra)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", "-"], nestedfileextra, sxmlfile2extra)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2"], nestedfileextra, sxmlfile2extra)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "3", "-"], nestedfileextra, sxmlfile3extra)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "3"], nestedfileextra, sxmlfile3extra)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.FILENAME_FLAG], nestedfileextra, "a.cpp\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.FILENAME_FLAG], nestedfileextra, "b.cpp\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.DIRECTORY_FLAG], nestedfileextra, "sub\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.DIRECTORY_FLAG], nestedfileextra, "sub\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.LANGUAGE_FLAG], nestedfileextra, "C\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.LANGUAGE_FLAG], nestedfileextra, "Java\n")

##
# src2srcml error return

# invalid input filename
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "foobar"], None), status.STATUS_INPUTFILE_PROBLEM)

# invalid input filename (repeat in output)
validate(getreturn([src2srcml, "sub/a.cpp", "-o", "sub/a.cpp"], None), status.STATUS_INPUTFILE_PROBLEM)

# unknown option
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--strip", "foobar"], None), status.STATUS_UNKNOWN_OPTION)

# unknown encoding

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TEXTENCODING_FLAG + "=" + bad_encoding, "foobar"], None), status.STATUS_UNKNOWN_ENCODING)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.ENCODING_FLAG + "=" + bad_encoding, "foobar"], None), status.STATUS_UNKNOWN_ENCODING)
	
# missing value
validate(getreturn([src2srcml, option.LANGUAGE_FLAG, bad_language, "foobar"], None), status.STATUS_INVALID_LANGUAGE)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG], None), status.STATUS_LANGUAGE_MISSING)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.FILENAME_FLAG], ""), status.STATUS_FILENAME_MISSING)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.DIRECTORY_FLAG], ""), status.STATUS_DIRECTORY_MISSING)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.SRCVERSION_FLAG], ""), status.STATUS_VERSION_MISSING)

# source encoding not given
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TEXTENCODING_FLAG], ""), status.STATUS_SRCENCODING_MISSING)

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.ENCODING_FLAG], ""), status.STATUS_XMLENCODING_MISSING)

##
# srcml2src error return

# invalid input filename
validate(getreturn([srcml2src, "foobar"], None), status.STATUS_INPUTFILE_PROBLEM)

# unknown option
validate(getreturn([srcml2src, "--strip", "foobar"], None), status.STATUS_UNKNOWN_OPTION)

# unknown encoding
validate(getreturn([srcml2src, option.TEXTENCODING_FLAG + "=" + bad_encoding], ""), status.STATUS_UNKNOWN_ENCODING)
validate(getreturn([srcml2src, option.TEXTENCODING_FLAG], ""), status.STATUS_SRCENCODING_MISSING)
	
# source encoding not given

# unit option selected but no value
validate(getreturn([srcml2src, option.UNIT_FLAG], ""), status.STATUS_UNIT_MISSING)

# unit value too large
missing_unit = "3";
validate(getreturn([srcml2src, option.UNIT_FLAG, missing_unit], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcml2src, option.UNIT_FLAG, missing_unit, option.XML_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcml2src, option.UNIT_FLAG, missing_unit, option.FILENAME_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcml2src, option.UNIT_FLAG, missing_unit, option.DIRECTORY_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcml2src, option.UNIT_FLAG, missing_unit, option.SRCVERSION_FLAG], nestedfile), status.STATUS_UNIT_INVALID)

# invalid combinations
validate(getreturn([srcml2src, option.XML_FLAG, option.TEXTENCODING_FLAG, "UTF-8", "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)

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
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++'], cpp_src, cpp_marked_srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_MARKUP_ELSE_FLAG], cpp_src, cpp_marked_srcml)


cpp_textonly_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
return;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_TEXTONLY_ELSE_FLAG], cpp_src, cpp_textonly_srcml)

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_MARKUP_ELSE_FLAG, option.CPP_TEXTONLY_ELSE_FLAG, "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_TEXTONLY_ELSE_FLAG, option.CPP_MARKUP_ELSE_FLAG, "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)

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
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++'], cpp_if0, cpp_textonly_srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_TEXTONLY_IF0_FLAG], cpp_if0, cpp_textonly_srcml)

cpp_marked_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
<break>break;</break>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_MARKUP_IF0_FLAG], cpp_if0, cpp_marked_srcml)

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_MARKUP_IF0_FLAG, option.CPP_TEXTONLY_IF0_FLAG, "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_TEXTONLY_IF0_FLAG, option.CPP_MARKUP_IF0_FLAG, "foobar"], None), status.STATUS_INVALID_OPTION_COMBINATION)

##
# xmlns options

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

# separate
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--xmlns=http://www.sdml.info/srcML/src"], "", srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--xmlns:cpp=http://www.sdml.info/srcML/cpp"], "", srcml)

# multiple
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp"], "", srcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
"""

# separate
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr"], "", srcml)

# multiple
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp"], "", srcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:err=http://www.sdml.info/srcML/srcerr"], "", srcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--debug", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr"], "", srcml)

srcml = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

srcml = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--xmlns:src=http://www.sdml.info/srcML/src", "--xmlns=http://www.sdml.info/srcML/cpp"], "", srcml)

##
# prefix extraction
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
"""

checkallforms(srcml2src, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/src", srcml, """
""")

checkallforms(srcml2src, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/cpp", srcml, """cpp
""")

checkallforms(srcml2src, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/literal", srcml, "")

checkallforms(srcml2src, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.ashland.edu/~mcollard/foo", srcml, "")

srcml = xml_declaration + """
<unit xmlns:cpp="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp"/>
"""

checkallforms(srcml2src, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/src", srcml, """cpp
""")

checkallforms(srcml2src, option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/cpp", srcml, """
""")

##
# no xml declaration
srcml = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.NO_XML_DECLARATION_FLAG], "", srcml)

##
# no namespace declaration
srcml = xml_declaration + """
<unit language="C++"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.NO_NAMESPACE_DECLARATION_FLAG], "", srcml)

##
# check missingfile
srcml = "Input:\tfoo.c\n" + src2srcml + ": Unable to open file foo.c\n"

checkError([src2srcml, 'foo.c'], "", srcml)

srcml = "Input:\tabc.c\n" + src2srcml + ": Unable to open file abc.c\n"

checkError([src2srcml, 'abc.c'], "", srcml)

srcml = "Input:\t../src/foo.c\n" + src2srcml +  ": Unable to open file ../src/foo.c\n"

checkError([src2srcml, '../src/foo.c'], "", srcml)

##
# check correct language based on file extension

xmltag = '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'
opentag = '<unit '
namespaceone = 'xmlns="http://www.sdml.info/srcML/src" '
namespacetwo = 'xmlns:cpp="http://www.sdml.info/srcML/cpp" '
fileopen = 'filename="emptysrc/'
fileclose = '"'
endtag = '/>\n'

# check c file extensions
language = 'language="C" '

check([src2srcml, 'emptysrc/empty.c'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.c' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.h'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.h' + fileclose + endtag)

check([src2srcml, 'emptysrc/empty.c.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.c.gz' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.h.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.h.gz' + fileclose + endtag)

# check c++ file extensions
language = 'language="C++" '

check([src2srcml, 'emptysrc/empty.cpp'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.cpp' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.cc'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.cc' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.cxx'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.cxx' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.c++'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.c++' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.hpp'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.hpp' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.hh'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.hh' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.hxx'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.hxx' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.h++'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.h++' + fileclose + endtag)

check([src2srcml, 'emptysrc/empty.cpp.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.cpp.gz' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.cc.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.cc.gz' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.cxx.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.cxx.gz' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.c++.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.c++.gz' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.hpp.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.hpp.gz' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.hh.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.hh.gz' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.hxx.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.hxx.gz' + fileclose + endtag)
check([src2srcml, 'emptysrc/empty.h++.gz'], "", xmltag + opentag + namespaceone + namespacetwo + language  + fileopen + 'empty.h++.gz' + fileclose + endtag)

# check java file extension
language = 'language="Java" '

check([src2srcml, 'emptysrc/empty.java'], "", xmltag + opentag + namespaceone + language  + fileopen + 'empty.java' + fileclose + endtag)

check([src2srcml, 'emptysrc/empty.java.gz'], "", xmltag + opentag + namespaceone + language  + fileopen + 'empty.java.gz' + fileclose + endtag)

# check aspectj file extension
language = 'language="AspectJ" '

check([src2srcml, 'emptysrc/empty.aj'], "", xmltag + opentag + namespaceone + language  + fileopen + 'empty.aj' + fileclose + endtag)

check([src2srcml, 'emptysrc/empty.aj.gz'], "", xmltag + opentag + namespaceone + language  + fileopen + 'empty.aj.gz' + fileclose + endtag)

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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

f = open('sub/a.cpp', 'w')
f.write(sfile)
f.close()

check([src2srcml, 'sub/a.cpp', '--output', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fxmlfile)
check([src2srcml, 'sub/a.cpp', '--output=sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fxmlfile)
check([src2srcml, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fxmlfile)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-', '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), sxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), sxmlfile)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-', '--output', '/dev/stdout'], sfile, sxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-', '--output=/dev/stdout'], sfile, sxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-', '-o', '/dev/stdout'], sfile, sxmlfile)

# srcml2src

check([srcml2src, 'sub/a.cpp.xml', '--output', 'sub/a.cpp'], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile)
check([srcml2src, 'sub/a.cpp.xml', '--output=sub/a.cpp'], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile)
check([srcml2src, 'sub/a.cpp.xml', '-o', 'sub/a.cpp'], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile)

check([srcml2src, '-', '-o', 'sub/a.cpp'], sxmlfile, "")
validate(open('sub/a.cpp', 'r').read(), sfile)
check([srcml2src, '-o', 'sub/a.cpp'], sxmlfile, "")
validate(open('sub/a.cpp', 'r').read(), sfile)

check([srcml2src, '-', '--output', '/dev/stdout'], sxmlfile, sfile)
check([srcml2src, '-', '--output=/dev/stdout'], sxmlfile, sfile)
check([srcml2src, '-', '-o', '/dev/stdout'], sxmlfile, sfile)

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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++" filename="sub/a.cpp"/>
"""
checkallformsfile(src2srcml, 'sub/a.cpp', option.DEBUG_FLAG_SHORT, option.DEBUG_FLAG, "", "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.DEBUG_FLAG_SHORT, '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.DEBUG_FLAG_SHORT, 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# language flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
"""
checkallformsfile(src2srcml, 'sub/a.cpp', option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C++", "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="sub/a.cpp"/>
"""
checkallformsfile(src2srcml, 'sub/a.cpp', option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C", "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="Java"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="Java" filename="sub/a.cpp"/>
"""
checkallformsfile(src2srcml, 'sub/a.cpp', option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "Java", "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'Java', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'Java', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# filename flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo.cpp"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo.cpp"/>
"""
checkallformsfile(src2srcml, 'sub/a.cpp', option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "foo.cpp", "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.FILENAME_FLAG_SHORT, 'foo.cpp', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.FILENAME_FLAG_SHORT, 'foo.cpp', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# directory flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="sub/a.cpp"/>
"""
checkallformsfile(src2srcml, 'sub/a.cpp', option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "bar", "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.DIRECTORY_FLAG_SHORT, 'bar', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.DIRECTORY_FLAG_SHORT, 'bar', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# version flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" version="1.0"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp" version="1.0"/>
"""
checkallformsfile(src2srcml, 'sub/a.cpp', option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "1.0", "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.SRCVERSION_FLAG_SHORT, '1.0', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.SRCVERSION_FLAG_SHORT, '1.0', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# xml encoding flag
srcml = """<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = """<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
"""
checkallformsfile(src2srcml, 'sub/a.cpp', option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "ISO-8859-1", "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.ENCODING_FLAG_SHORT, 'ISO-8859-1', '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.ENCODING_FLAG_SHORT, 'ISO-8859-1', 'sub/a.cpp','-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# text encoding flag
srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
"""
checkallforms(src2srcml, option.TEXTENCODING_FLAG_SHORT, option.TEXTENCODING_FLAG, "ISO-8859-1", sfile1, srcml)
checkallformsfile(src2srcml, 'sub/a.cpp', option.TEXTENCODING_FLAG_SHORT, option.TEXTENCODING_FLAG, "ISO-8859-1", "", fsrcml)
check([src2srcml, option.TEXTENCODING_FLAG, "ISO-8859-1", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TEXTENCODING_FLAG, "ISO-8859-1", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.TEXTENCODING_FLAG, "ISO-8859-1", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

# nested 

sfile1 = """
a;
"""

sfile2 = """
b;
"""


nestedfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfilesrc = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src">

<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
</src:unit>

<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
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

check([src2srcml, option.COMPOUND_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), nestedfile1)
check([src2srcml, 'sub/a.cpp', 'sub/b.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), nestedfile)
check([src2srcml, '--xmlns:src=http://www.sdml.info/srcML/src', 'sub/a.cpp', 'sub/b.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), nestedfilesrc)

# files from
nestedfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
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

check([src2srcml, option.FILELIST_FLAG, "filelistab", '-o', 'sub/a.cpp.xml'], "", "")
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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
"""

f = open('sub/a.cpp', 'w')
f.write(sfile1)
f.close()

# separate
check([src2srcml, "--xmlns=http://www.sdml.info/srcML/src", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--xmlns=http://www.sdml.info/srcML/src", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, "--xmlns=http://www.sdml.info/srcML/src", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)
check([src2srcml, "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--xmlns:cpp=http://www.sdml.info/srcML/cpp", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

# multiple
check([src2srcml, "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:err="http://www.sdml.info/srcML/srcerr" language="C++" filename="sub/a.cpp"/>
"""

# separate
check([src2srcml, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

# multiple
check([src2srcml, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:err=http://www.sdml.info/srcML/srcerr", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, "--debug", "--xmlns=http://www.sdml.info/srcML/src", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, "--debug", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--debug", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, "--debug", "--xmlns:cpp=http://www.sdml.info/srcML/cpp", "--xmlns:err=http://www.sdml.info/srcML/srcerr", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src" xmlns="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
"""
check([src2srcml, "--xmlns:src=http://www.sdml.info/srcML/src", "--xmlns=http://www.sdml.info/srcML/cpp", 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', "--xmlns:src=http://www.sdml.info/srcML/src", "--xmlns=http://www.sdml.info/srcML/cpp", '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, "--xmlns:src=http://www.sdml.info/srcML/src", "--xmlns=http://www.sdml.info/srcML/cpp", 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# no xml declaration
srcml = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

fsrcml = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.NO_XML_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

##
# no namespace declaration
srcml = xml_declaration + """
<unit language="C++"/>
"""

fsrcml = xml_declaration + """
<unit language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.NO_NAMESPACE_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], sfile1, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
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

check([src2srcml, 'sub/a.cpp'], "", fcpp_marked_srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-o', 'sub/a.cpp.xml'], cpp_src, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_marked_srcml)
check([src2srcml, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_marked_srcml)

check([src2srcml, option.CPP_MARKUP_ELSE_FLAG, 'sub/a.cpp'], "", fcpp_marked_srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_MARKUP_ELSE_FLAG, '-o', 'sub/a.cpp.xml'], cpp_src, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_marked_srcml)
check([src2srcml, option.CPP_MARKUP_ELSE_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
<break>break;</break>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
return;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

check([src2srcml, option.CPP_TEXTONLY_ELSE_FLAG, 'sub/a.cpp'], "", fcpp_textonly_srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_TEXTONLY_ELSE_FLAG, '-o', 'sub/a.cpp.xml'], cpp_src, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_textonly_srcml)
check([src2srcml, option.CPP_TEXTONLY_ELSE_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
break;
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

f = open('sub/a.cpp', 'w')
f.write(cpp_if0)
f.close()

check([src2srcml, 'sub/a.cpp'], "", fcpp_textonly_srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-o', 'sub/a.cpp.xml'], cpp_if0, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_textonly_srcml)
check([src2srcml, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_textonly_srcml)

check([src2srcml, option.CPP_TEXTONLY_IF0_FLAG, 'sub/a.cpp'], "", fcpp_textonly_srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_TEXTONLY_IF0_FLAG, '-o', 'sub/a.cpp.xml'], cpp_if0, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_textonly_srcml)
check([src2srcml, option.CPP_TEXTONLY_IF0_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_textonly_srcml)

cpp_marked_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
<break>break;</break>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

fcpp_marked_srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<cpp:if>#<cpp:directive>if</cpp:directive> <expr>0</expr></cpp:if>
<break>break;</break>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
</unit>
"""

check([src2srcml, option.CPP_MARKUP_IF0_FLAG, 'sub/a.cpp'], "", fcpp_marked_srcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.CPP_MARKUP_IF0_FLAG, '-o', 'sub/a.cpp.xml'], cpp_if0, "")
validate(open('sub/a.cpp.xml', 'r').read(), cpp_marked_srcml)
check([src2srcml, option.CPP_MARKUP_IF0_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fcpp_marked_srcml)

##
# Test srcml2src options with files

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" version="1.2">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

nestedfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfilesrc = xml_declaration + """
<src:unit xmlns:src="http://www.sdml.info/srcML/src">

<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
</src:unit>

<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
</src:unit>

</src:unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(sxmlfile1)
f.close()
# check metadata options
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", "", "C++\n")
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", "", "sub\n")
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", "", "a.cpp\n")
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", "", "1.2\n")
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "", "", default_srcml2src_encoding + "\n")

check([srcml2src, option.NESTED_FLAG, 'sub/a.cpp.xml'], "", "1\n")

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile1)
f.close()
check([srcml2src, option.NESTED_FLAG, 'sub/a.cpp.xml'], "", "1\n")

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile)
f.close()
check([srcml2src, option.NESTED_FLAG, 'sub/a.cpp.xml'], "", "2\n")

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfilesrc)
f.close()
check([srcml2src, option.NESTED_FLAG, 'sub/a.cpp.xml'], "", "2\n")

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
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile)
f.close()

checkallformsfile(srcml2src, 'sub/a.cpp.xml', "-U", option.UNIT_FLAG, "1", "", sfile1)
check([srcml2src, option.UNIT_FLAG, "1", '-o', "sub/a.cpp"], nestedfile, "")
validate(open('sub/a.cpp', 'r').read(), sfile1)
check([srcml2src, option.UNIT_FLAG, "1", 'sub/a.cpp.xml', '-o', "sub/a.cpp"], "", "")
validate(open('sub/a.cpp', 'r').read(), sfile1)

checkallformsfile(srcml2src, 'sub/a.cpp.xml', "-U", option.UNIT_FLAG, "2", "", sfile2)
check([srcml2src, option.UNIT_FLAG, "2", '-o', "sub/b.cpp"], nestedfile, "")
validate(open('sub/b.cpp', 'r').read(), sfile2)
check([srcml2src, option.UNIT_FLAG, "2", 'sub/a.cpp.xml', '-o', "sub/b.cpp"], "", "")
validate(open('sub/b.cpp', 'r').read(), sfile2)

# check xml and unit option

check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", 'sub/a.cpp.xml'], "", sxmlfile1)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", '-o', 'sub/b.cpp.xml'], nestedfile, "")
validate(open('sub/b.cpp.xml', 'r').read(), sxmlfile1)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", 'sub/a.cpp.xml', '-o', "sub/b.cpp.xml"], "", "")
validate(open('sub/b.cpp.xml', 'r').read(), sxmlfile1)

check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", 'sub/a.cpp.xml'], "", sxmlfile2)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", '-o', 'sub/b.cpp.xml'], nestedfile, "")
validate(open('sub/b.cpp.xml', 'r').read(), sxmlfile2)
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", 'sub/a.cpp.xml', '-o', "sub/b.cpp.xml"], "", "")
validate(open('sub/b.cpp.xml', 'r').read(), sxmlfile2)

# check metadata options with xml and unit

nestedfileextra = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" mytag="foo">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java" dir="emptysrc" mytag="foo" filename="empty.java">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfileextra)
f.close()

check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.LANGUAGE_FLAG, 'sub/a.cpp.xml'], "", "C++\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.DIRECTORY_FLAG, 'sub/a.cpp.xml'], "", "sub\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "1", option.FILENAME_FLAG, 'sub/a.cpp.xml'], "", "a.cpp\n")

check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.LANGUAGE_FLAG, 'sub/a.cpp.xml'], "", "Java\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.DIRECTORY_FLAG, 'sub/a.cpp.xml'], "", "emptysrc\n")
check([srcml2src, option.XML_FLAG, option.UNIT_FLAG, "2", option.FILENAME_FLAG, 'sub/a.cpp.xml'], "", "empty.java\n")

# prefix extraction

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(sxmlfile1)
f.close()
execute([src2srcml, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "")
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/src", "", """
""")

checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/cpp", "", """cpp
""")

checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.sdml.info/srcML/literal", "", "")

checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.NAMESPACE_FLAG_SHORT, option.NAMESPACE_FLAG, "http://www.ashland.edu/~mcollard/foo", "", "")

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
checkallforms(srcml2src, option.TEXTENCODING_FLAG_SHORT, option.TEXTENCODING_FLAG, "ISO-8859-1", sxmlfile1, sfile1)
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.TEXTENCODING_FLAG_SHORT, option.TEXTENCODING_FLAG, "ISO-8859-1", "", sfile1)
check([srcml2src, option.TEXTENCODING_FLAG, "ISO-8859-1", 'sub/a.cpp.xml'], "", sfile1)
check([srcml2src, option.TEXTENCODING_FLAG, "ISO-8859-1", '-o', 'sub/a.cpp'], sxmlfile1, "")
validate(open('sub/a.cpp', 'r').read(), sfile1)
check([srcml2src, option.TEXTENCODING_FLAG, "ISO-8859-1", 'sub/a.cpp.xml', '-o', 'sub/a.cpp'], "", "")
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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

# src2srcml

f = open('sub/a.cpp', 'w')
f.write(sfile)
f.close()
check([src2srcml, option.COMPRESSED_FLAG_SHORT, 'sub/a.cpp', '-o', 'sub/a.cpp.xml.gz'], "", "")
check(['gunzip', '-c', 'sub/a.cpp.xml.gz'], "", fxmlfile)
check([src2srcml, option.COMPRESSED_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml.gz'], "", "")
check(['gunzip', '-c', 'sub/a.cpp.xml.gz'], "", fxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.COMPRESSED_FLAG_SHORT, '-o', 'sub/a.cpp.xml.gz'], sfile, "")
check(['gunzip', '-c', 'sub/a.cpp.xml.gz'], "", sxmlfile)


# srcml2src

f = open('sub/a.cpp.xml', 'w')
f.write(fxmlfile)
f.close()
check([srcml2src, option.COMPRESSED_FLAG_SHORT, 'sub/a.cpp.xml', '-o', 'sub/a.cpp.gz'], "", "")
check(['gunzip', '-c', 'sub/a.cpp.gz'], "", sfile)
check([srcml2src, option.COMPRESSED_FLAG, 'sub/a.cpp.xml', '-o', 'sub/a.cpp.gz'], "", "")
check(['gunzip', '-c', 'sub/a.cpp.gz'], "", sfile)
check([srcml2src, option.COMPRESSED_FLAG_SHORT, '-o', 'sub/a.cpp.gz'], fxmlfile, "")
check(['gunzip', '-c', 'sub/a.cpp.gz'], "", sfile)

# test input file is gzipped

sfile = """
a;
"""

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

fxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp.gz">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

f = open('sub/a.cpp.gz', 'r')
gzipped = f.read()
f.close()

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++'], gzipped, sxmlfile)
check([src2srcml, 'sub/a.cpp.gz'], "", fxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', '-o', 'sub/a.cpp.xml'], gzipped, "")
validate(open('sub/a.cpp.xml', 'r').read(), sxmlfile)
check([src2srcml, 'sub/a.cpp.gz', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fxmlfile)

f = open('sub/a.cpp.xml.gz', 'r')
gzipped = f.read()
f.close()

check([srcml2src], gzipped, sfile)
check([srcml2src, 'sub/a.cpp.xml.gz'], "", sfile)
check([srcml2src, '-o', 'sub/a.cpp'], gzipped, "")
validate(open('sub/a.cpp', 'r').read(), sfile)
check([srcml2src, 'sub/a.cpp.xml.gz', '-o', 'sub/a.cpp'], "", "")
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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG], sfile, srcml)
check([src2srcml, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG], sfile, srcml)
check([src2srcml, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG], sfile, srcml)
check([src2srcml, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, option.OPERATOR_FLAG], sfile, srcml)
check([src2srcml, option.LITERAL_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.LITERAL_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, option.LITERAL_FLAG], sfile, srcml)
check([src2srcml, option.OPERATOR_FLAG, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.OPERATOR_FLAG, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, option.MODIFIER_FLAG], sfile, srcml)
check([src2srcml, option.LITERAL_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.LITERAL_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, option.LITERAL_FLAG], sfile, srcml)
check([src2srcml, option.MODIFIER_FLAG, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.MODIFIER_FLAG, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, option.MODIFIER_FLAG], sfile, srcml)
check([src2srcml, option.OPERATOR_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.OPERATOR_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, option.OPERATOR_FLAG], sfile, srcml)
check([src2srcml, option.MODIFIER_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.MODIFIER_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++"/>
"""

fsrcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:lit="http://www.sdml.info/srcML/literal" xmlns:op="http://www.sdml.info/srcML/operator" xmlns:type="http://www.sdml.info/srcML/modifier" language="C++" filename="sub/a.cpp"/>
"""

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, option.OPERATOR_FLAG, option.MODIFIER_FLAG], sfile, srcml)
check([src2srcml, option.LITERAL_FLAG, option.OPERATOR_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, option.OPERATOR_FLAG, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.LITERAL_FLAG, option.OPERATOR_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, option.MODIFIER_FLAG, option.OPERATOR_FLAG], sfile, srcml)
check([src2srcml, option.LITERAL_FLAG, option.MODIFIER_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.LITERAL_FLAG, option.MODIFIER_FLAG, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.LITERAL_FLAG, option.MODIFIER_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, option.LITERAL_FLAG, option.MODIFIER_FLAG], sfile, srcml)
check([src2srcml, option.OPERATOR_FLAG, option.LITERAL_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, option.LITERAL_FLAG, option.MODIFIER_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.OPERATOR_FLAG, option.LITERAL_FLAG, option.MODIFIER_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, option.MODIFIER_FLAG, option.LITERAL_FLAG], sfile, srcml)
check([src2srcml, option.OPERATOR_FLAG, option.MODIFIER_FLAG, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.OPERATOR_FLAG, option.MODIFIER_FLAG, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.OPERATOR_FLAG, option.MODIFIER_FLAG, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, option.LITERAL_FLAG, option.OPERATOR_FLAG], sfile, srcml)
check([src2srcml, option.MODIFIER_FLAG, option.LITERAL_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, option.LITERAL_FLAG, option.OPERATOR_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.MODIFIER_FLAG, option.LITERAL_FLAG, option.OPERATOR_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml', 'r').read(), fsrcml)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, option.OPERATOR_FLAG, option.LITERAL_FLAG], sfile, srcml)
check([src2srcml, option.MODIFIER_FLAG, option.OPERATOR_FLAG, option.LITERAL_FLAG, 'sub/a.cpp'], "", fsrcml)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.MODIFIER_FLAG, option.OPERATOR_FLAG, option.LITERAL_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
validate(open('sub/a.cpp.xml', 'r').read(), srcml)
check([src2srcml, option.MODIFIER_FLAG, option.OPERATOR_FLAG, option.LITERAL_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
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
directory="sub"
filename="a.cpp"
files="1"
"""

longinfonested = """xmlns="http://www.sdml.info/srcML/src"
encoding="UTF-8"
files="2"
"""

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

nestedfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(sxmlfile1)
f.close()
checkallforms(srcml2src, option.INFO_FLAG_SHORT, option.INFO_FLAG, "", sxmlfile, info)
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.INFO_FLAG_SHORT, option.INFO_FLAG, "", "", info)
checkallforms(srcml2src, option.LONG_INFO_FLAG_SHORT, option.LONG_INFO_FLAG, "", sxmlfile, longinfo)
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.LONG_INFO_FLAG_SHORT, option.LONG_INFO_FLAG, "", "", longinfo)

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile)
f.close()
checkallforms(srcml2src, option.LONG_INFO_FLAG_SHORT, option.LONG_INFO_FLAG, "", nestedfile, longinfonested)
checkallformsfile(srcml2src, 'sub/a.cpp.xml', option.LONG_INFO_FLAG_SHORT, option.LONG_INFO_FLAG, "", "", longinfonested)

##
# test extract all command

sfile1 = """
a;
"""

sfile2 = """
b;
"""

srcml = xml_declaration + """ 
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

nestedfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

os.system("rm -f sub/a.cpp")

checkNoOutput([srcml2src, option.EXPAND_FLAG + '=.', 'sub/a.cpp.xml'], srcml)

validate(open("sub/a.cpp", "r").read(), sfile1)

f = open('sub/a.cpp.xml', 'w')
f.write(nestedfile)
f.close()

os.system("rm -f sub/a.cpp sub/b.cpp")

checkNoOutput([srcml2src, option.EXPAND_FLAG + '=.', 'sub/a.cpp.xml'], "")
validate(open('sub/a.cpp', 'r').read(), sfile1)
validate(open('sub/b.cpp', 'r').read(), sfile2)

#os.system("rm -f sub/a.cpp sub/b.cpp")

checkNoOutput([srcml2src, option.EXPAND_FLAG_SHORT, '.', 'sub/a.cpp.xml'], "")
#validate(open('sub/a.cpp', 'r').read(), sfile1)
#validate(open('sub/b.cpp', 'r').read(), sfile2)

##
# xml flag

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

check([srcml2src, option.XML_FLAG], srcml, srcml)
check([srcml2src, option.XML_FLAG, 'sub/a.cpp.xml'], "", srcml)
check([srcml2src, option.XML_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcml)
check([srcml2src, option.XML_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcml)

##
# no xml declaration srcml2src

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

srcmlout = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

check([srcml2src, option.XML_FLAG, option.NO_XML_DECLARATION_FLAG], srcml, srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_XML_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

check([srcml2src, option.NO_XML_DECLARATION_FLAG, option.XML_FLAG], srcml, srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, option.XML_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, option.XML_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, option.XML_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

check([srcml2src, option.NO_XML_DECLARATION_FLAG], srcml, srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
</unit>
"""

srcmlout = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

check([srcml2src, option.XML_FLAG, option.NO_XML_DECLARATION_FLAG], srcml, srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_XML_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

check([srcml2src, option.NO_XML_DECLARATION_FLAG, option.XML_FLAG], srcml, srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, option.XML_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, option.XML_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, option.XML_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

check([srcml2src, option.NO_XML_DECLARATION_FLAG], srcml, srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, '-o', 'sub/a.cpp.xml'], srcml, "")
validate(open('sub/a.cpp.xml').read(), srcmlout)
check([srcml2src, option.NO_XML_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

##
# no namespace declaration

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

srcmlout = xml_declaration + """
<unit language="C++"/>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

check([srcml2src, option.XML_FLAG, option.NO_NAMESPACE_DECLARATION_FLAG], srcml, srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_NAMESPACE_DECLARATION_FLAG, '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml').read(), srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, option.XML_FLAG], srcml, srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, option.XML_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, option.XML_FLAG, '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml').read(), srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, option.XML_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG], srcml, srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml').read(), srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
</unit>
"""

srcmlout = xml_declaration + """
<unit language="C++">
</unit>
"""

f = open('sub/a.cpp.xml', 'w')
f.write(srcml)
f.close()

check([srcml2src, option.XML_FLAG, option.NO_NAMESPACE_DECLARATION_FLAG], srcml, srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_NAMESPACE_DECLARATION_FLAG, '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml').read(), srcmlout)
check([srcml2src, option.XML_FLAG, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, option.XML_FLAG], srcml, srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, option.XML_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, option.XML_FLAG, '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml').read(), srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, option.XML_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG], srcml, srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml'], "", srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml').read(), srcmlout)
check([srcml2src, option.NO_NAMESPACE_DECLARATION_FLAG, 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml').read(), srcmlout)

##
# Help and version


# src2srcml

globals()["test_count"] += 1
print test_count, os.path.basename(src2srcml) + ' ' + option.HELP_FLAG
line = execute([src2srcml, option.HELP_FLAG], "")
execute(['grep', 'Report bugs'], line)
globals()["test_count"] += 1
print test_count, os.path.basename(src2srcml) + ' ' + option.VERSION_FLAG
line = execute([src2srcml, option.VERSION_FLAG], "")
execute(['grep', 'Copyright'], line)

# srcML2src
globals()["test_count"] += 1
print test_count, os.path.basename(srcml2src) + ' ' + option.HELP_FLAG
line = execute([srcml2src, option.HELP_FLAG], "")
execute(['grep', 'Report bugs'], line)
globals()["test_count"] += 1
print test_count, os.path.basename(srcml2src) + ' ' + option.VERSION_FLAG
line = execute([srcml2src, option.VERSION_FLAG], "")
execute(['grep', 'Copyright'], line)

##
# Test order of metadata option order

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp" version="1.0"/>
"""

options = [option.LANGUAGE_FLAG_SHORT, option.DIRECTORY_FLAG_SHORT, option.FILENAME_FLAG_SHORT, option.SRCVERSION_FLAG_SHORT, option.ENCODING_FLAG_SHORT,
	   option.LANGUAGE_FLAG_SHORT, option.DIRECTORY_FLAG_SHORT, option.FILENAME_FLAG_SHORT, option.SRCVERSION_FLAG_SHORT, option.ENCODING_FLAG_SHORT]

values = ['language="C++"\n', 'directory="sub"\n', 'filename="a.cpp"\n', 'src-version="1.0"\n', 'encoding="UTF-8"\n',
	  'language="C++"\n', 'directory="sub"\n', 'filename="a.cpp"\n', 'src-version="1.0"\n', 'encoding="UTF-8"\n']

index = 0
check([srcml2src, options[index] + options[index + 1][1] + options[index + 2][1] + options[index + 3][1] + options[index + 4][1]], srcml, values[index] + values[index + 1] + values[index + 2] + values[index + 3] + values[index + 4])

index += 1
check([srcml2src, options[index] + options[index + 1][1] + options[index + 2][1] + options[index + 3][1] + options[index + 4][1]], srcml, values[index] + values[index + 1] + values[index + 2] + values[index + 3] + values[index + 4])

index += 1
check([srcml2src, options[index] + options[index + 1][1] + options[index + 2][1] + options[index + 3][1] + options[index + 4][1]], srcml, values[index] + values[index + 1] + values[index + 2] + values[index + 3] + values[index + 4])

index += 1
check([srcml2src, options[index] + options[index + 1][1] + options[index + 2][1] + options[index + 3][1] + options[index + 4][1]], srcml, values[index] + values[index + 1] + values[index + 2] + values[index + 3] + values[index + 4])

index += 1
check([srcml2src, options[index] + options[index + 1][1] + options[index + 2][1] + options[index + 3][1] + options[index + 4][1]], srcml, values[index] + values[index + 1] + values[index + 2] + values[index + 3] + values[index + 4])

##
# Testing for verbose

sfile = """
a;
"""

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

srcencoding = """
Source Encoding: """ + default_src2srcml_encoding

xmlencoding = """
XML Encoding: """ + default_srcml2src_encoding

f = open('sub/a.cpp', 'w')
f.write(sfile)
f.close

f = open('sub/a.cpp.xml', 'w')
f.write(sxmlfile)
f.close()
 
# src2srcml

globals()["test_count"] += 1
print test_count, os.path.basename(src2srcml) + ' ' + option.VERBOSE_FLAG
line = execute([src2srcml, option.VERBOSE_FLAG, option.LANGUAGE_FLAG_SHORT, 'C++'], sfile)
execute(['grep', srcencoding + xmlencoding], line)
globals()["test_count"] += 1
print test_count, os.path.basename(src2srcml) + ' ' + option.VERBOSE_FLAG + ' sub/a.cpp'
line = execute([src2srcml, option.VERBOSE_FLAG, 'sub/a.cpp'], "")
execute(['grep', srcencoding + xmlencoding], line)

# srcml2src
globals()["test_count"] += 1
print test_count, os.path.basename(srcml2src) + ' ' + option.VERBOSE_FLAG
line = execute([srcml2src, option.VERBOSE_FLAG], sxmlfile)
execute(['grep', xmlencoding + srcencoding], line)
globals()["test_count"] += 1
print test_count, os.path.basename(srcml2src) + ' ' + option.VERBOSE_FLAG + ' sub/a.cpp.xml'
line = execute([srcml2src, option.VERBOSE_FLAG, 'sub/a.cpp.xml'], "")
execute(['grep', xmlencoding + srcencoding], line)

##
# src2srcml expression option

sfile ="""
a
"""

sxmlfile = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr><name>a</name></expr>
</unit>
"""

fsxmlfile = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr><name>a</name></expr>
</unit>
"""

file = open('sub/a.cpp', 'w')
file.write(sfile)
file.close()

#checkallforms(src2srcml, option.EXPRESSION_MODE_FLAG_SHORT, option.EXPRESSION_MODE_FLAG, "", sfile, sxmlfile)
#checkallformsfile(src2srcml, 'sub/a.cpp', option.EXPRESSION_MODE_FLAG_SHORT, option.EXPRESSION_MODE_FLAG, "", "", fsxmlfile)
#check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.EXPRESSION_MODE_FLAG, '-o', 'sub/a.cpp.xml'], sfile, "")
#validate(open('sub/a.cpp.xml', 'r').read(), sxmlfile)
#check([src2srcml, option.EXPRESSION_MODE_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
#validate(open('sub/a.cpp.xml', 'r').read(), fsxmlfile)

##
# Test Query and Transformation Options

# xpath

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

xpath = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

file = open('sub/a.cpp.xml', 'w')
file.write(srcml)
file.close()

check([srcml2src, option.XPATH_FLAG + '=src:unit'], srcml, xpath)
check([srcml2src, option.XPATH_FLAG + '=src:unit', 'sub/a.cpp.xml'], "", xpath)
check([srcml2src, option.XPATH_FLAG + '=src:unit', '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml', 'r').read(), xpath)
check([srcml2src, option.XPATH_FLAG + '=src:unit', 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml', 'r').read(), xpath)

validate(getreturn([srcml2src, option.XPATH_FLAG], srcml), status.STATUS_ERROR)
validate(getreturn([srcml2src, option.XPATH_FLAG + '='], srcml), status.STATUS_ERROR)

# xslt and param

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

xslt = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

file = open('sub/a.cpp.xml', 'w')
file.write(srcml)
file.close()

# xslt

check([srcml2src, option.XSLT_FLAG + '=copy.xsl'], srcml, xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', 'sub/a.cpp.xml'], "", xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml', 'r').read(), xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml', 'r').read(), xpath)

validate(getreturn([srcml2src, option.XSLT_FLAG], srcml), status.STATUS_ERROR)
validate(getreturn([srcml2src, option.XSLT_FLAG + '='], srcml), status.STATUS_ERROR)

# param

check([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.PARAM_FLAG, 'NAME=VALUE'], srcml, xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.PARAM_FLAG, 'NAME=VALUE', 'sub/a.cpp.xml'], "", xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.PARAM_FLAG, 'NAME=VALUE', '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml', 'r').read(), xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.PARAM_FLAG, 'NAME=VALUE', 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml', 'r').read(), xpath)

validate(getreturn([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.PARAM_FLAG], srcml), status.STATUS_ERROR)
validate(getreturn([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.PARAM_FLAG, "NAME"], srcml), status.STATUS_ERROR)

# stringparam

check([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.STRING_PARAM_FLAG, 'NAME=VALUE'], srcml, xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.STRING_PARAM_FLAG, 'NAME=VALUE', 'sub/a.cpp.xml'], "", xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.STRING_PARAM_FLAG, 'NAME=VALUE', '-o', 'sub/b.cpp.xml'], srcml, "")
validate(open('sub/b.cpp.xml', 'r').read(), xpath)
check([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.STRING_PARAM_FLAG, 'NAME=VALUE', 'sub/a.cpp.xml', '-o', 'sub/b.cpp.xml'], "", "")
validate(open('sub/b.cpp.xml', 'r').read(), xpath)

validate(getreturn([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.STRING_PARAM_FLAG], srcml), status.STATUS_ERROR)
validate(getreturn([srcml2src, option.XSLT_FLAG + '=copy.xsl', option.STRING_PARAM_FLAG, "NAME"], srcml), status.STATUS_ERROR)

# relaxng

# position

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="8"/>
"""

fsxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename="sub/a.cpp" pos:tabs="8"/>
"""

f = open('sub/a.cpp', 'w')
f.write("")
f.close()

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.POSITION_FLAG], "", sxmlfile)
check([src2srcml, option.POSITION_FLAG, 'sub/a.cpp'], "", fsxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.POSITION_FLAG, '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), sxmlfile)
check([src2srcml, option.POSITION_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), fsxmlfile)

# tabs

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="4"/>
"""

fsxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename="sub/a.cpp" pos:tabs="4"/>
"""

f = open('sub/a.cpp', 'w')
f.write("")
f.close()

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TABS_FLAG, '4'], "", sxmlfile)
check([src2srcml, option.TABS_FLAG, '4', 'sub/a.cpp'], "", fsxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TABS_FLAG, '4', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), sxmlfile)
check([src2srcml, option.TABS_FLAG, '4', 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), fsxmlfile)

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TABS_FLAG], ""), status.STATUS_ERROR)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TABS_FLAG, 'a'], ""), status.STATUS_UNIT_INVALID)

# position and tabs

sxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" pos:tabs="2"/>
"""

fsxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:pos="http://www.sdml.info/srcML/position" language="C++" filename="sub/a.cpp" pos:tabs="2"/>
"""

f = open('sub/a.cpp', 'w')
f.write("")
f.close()

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.POSITION_FLAG, option.TABS_FLAG, '2'], "", sxmlfile)
check([src2srcml, option.POSITION_FLAG, option.TABS_FLAG, '2', 'sub/a.cpp'], "", fsxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.POSITION_FLAG, option.TABS_FLAG, '2', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), sxmlfile)
check([src2srcml, option.POSITION_FLAG, option.TABS_FLAG, '2', 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), fsxmlfile)

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.POSITION_FLAG, option.TABS_FLAG], ""), status.STATUS_ERROR)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.POSITION_FLAG, option.TABS_FLAG, 'a'], ""), status.STATUS_UNIT_INVALID)

check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TABS_FLAG, '2', option.POSITION_FLAG], "", sxmlfile)
check([src2srcml, option.TABS_FLAG, '2', option.POSITION_FLAG, 'sub/a.cpp'], "", fsxmlfile)
check([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TABS_FLAG, '2', option.POSITION_FLAG, '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), sxmlfile)
check([src2srcml, option.TABS_FLAG, '2', option.POSITION_FLAG, 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), fsxmlfile)

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TABS_FLAG, option.POSITION_FLAG], ""), status.STATUS_UNIT_INVALID)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.TABS_FLAG, 'a', option.POSITION_FLAG], ""), status.STATUS_UNIT_INVALID)

# register language

fsxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="Java" filename="sub/a.cpp"/>
"""

f = open('sub/a.cpp', 'w')
f.write("")
f.close()

check([src2srcml, option.REGISTER_EXT_FLAG, 'cpp=Java', 'sub/a.cpp'], "", fsxmlfile)
check([src2srcml, option.REGISTER_EXT_FLAG + '=cpp=Java', 'sub/a.cpp'], "", fsxmlfile)
check([src2srcml, option.REGISTER_EXT_FLAG, 'cpp=Java', 'sub/a.cpp', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), fsxmlfile)

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.REGISTER_EXT_FLAG], ""), status.STATUS_ERROR)
validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.REGISTER_EXT_FLAG, "cpp=Jawa"], ""), status.STATUS_ERROR)

fsxmlfile = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="Java" filename="sub/a.xml"/>
"""

os.system("touch sub/a.xml")
check([src2srcml, option.REGISTER_EXT_FLAG, 'xml=Java', 'sub/a.xml'], "", fsxmlfile)
check([src2srcml, option.REGISTER_EXT_FLAG + '=xml=Java', 'sub/a.xml'], "", fsxmlfile)
check([src2srcml, option.REGISTER_EXT_FLAG, 'xml=Java', 'sub/a.xml', '-o', 'sub/a.cpp.xml'], "", "")
validate(open('sub/a.cpp.xml').read(), fsxmlfile)

validate(getreturn([src2srcml, option.LANGUAGE_FLAG_SHORT, 'C++', option.REGISTER_EXT_FLAG, "xml=Jawa"], ""), status.STATUS_ERROR)

##
# directory input

src = """
a;
"""

srcmlstart = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">
"""

aj = """
<unit language="AspectJ" filename="dir/file.aj">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

c = """
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="dir/file.c">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

cpp = """
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="dir/file.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

java = """
<unit language="Java" filename="dir/file.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

srcmlend = """
</unit>
"""

if os.path.exists("dir"):
	os.system("rm -r dir; mkdir dir")
else :
	os.system("mkdir dir")

f = open("dir/file.aj", 'w')
f.write(src)
f.close()

f = open("dir/file.c", 'w')
f.write(src)
f.close()

f = open("dir/file.cpp", 'w')
f.write(src)
f.close()

f = open("dir/file.java", 'w')
f.write(src)
f.close()

line = execute(['ls', '-f1', 'dir'], "")

srcml = srcmlstart

for file in line.split('\n') :
	if file == 'file.aj' :
		srcml += aj
	if file == 'file.c' :
		srcml += c
	if file == 'file.cpp' :
		srcml += cpp
	if file == 'file.java' :
		srcml += java

srcml += srcmlend

check([src2srcml, 'dir'], "", srcml)
check([src2srcml, 'dir', '-o', 'dir/dir.xml'], "", "")
validate(open('dir/dir.xml', 'r').read(), srcml)

#
# nested files

src = """
a;
"""

srcmlstart = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">
"""

cpp = """
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

cppempty = """
<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
"""

java = """
<unit language="C++" filename="sub/a.java">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

javaempty = """
<unit language="C++" filename="sub/a.java"/>
"""

srcmlend = """
</unit>
"""

os.system('rm sub/a.cpp; touch sub/a.cpp')
f = open('sub/a.java', 'w')
f.write(src)
f.close

check([src2srcml, 'sub/a.cpp', 'sub/a.java'], '', srcmlstart + cppempty + java + srcmlend)
check([src2srcml, 'sub/a.cpp', 'sub/a.java', '-o', 'sub/all.xml'], '', '')
validate(open('sub/all.xml', 'r').read(), srcmlstart + cppempty + java + srcmlend)

check([src2srcml, 'sub/a.java', 'sub/a.cpp'], '', srcmlstart + java + cppempty + srcmlend)
check([src2srcml, 'sub/a.java', 'sub/a.cpp', '-o', 'sub/all.xml'], '', '')
validate(open('sub/all.xml', 'r').read(), srcmlstart + java + cppempty + srcmlend)

os.system('rm sub/a.java; touch sub/a.java')
f = open('sub/a.cpp', 'w')
f.write(src)
f.close

check([src2srcml, 'sub/a.cpp', 'sub/a.java'], '', srcmlstart + cppempty + java + srcmlend)
check([src2srcml, 'sub/a.cpp', 'sub/a.java', '-o', 'sub/all.xml'], '', '')
validate(open('sub/all.xml', 'r').read(), srcmlstart + cppempty + java + srcmlend)

check([src2srcml, 'sub/a.java', 'sub/a.cpp'], '', srcmlstart + java + cppempty + srcmlend)
check([src2srcml, 'sub/a.java', 'sub/a.cpp', '-o', 'sub/all.xml'], '', '')
validate(open('sub/all.xml', 'r').read(), srcmlstart + java + cppempty + srcmlend)

# footer
print
print "Error count: ", error_count
print
print src2srcmlversion()
print srcml2srcversion()

exit
