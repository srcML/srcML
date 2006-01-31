#!/usr/bin/python2.4
#
# clitest.py
#
# Michael L. Collard
# collard@cs.kent.edu

import sys
import os.path
import string
import re
import subprocess
import difflib

import option
import status

debug = 0

def check(command, input, output):

	print os.path.basename(command[0]), ' '.join(command[1:])
	
	line = execute(command, input)

	validate(string.strip(output), string.strip(line))
	
def validate(org, gen):
	if org != gen:
		print "ERROR"
		print "org|" + str(org) + "|"
		print "gen|" + str(gen) + "|"
	return

def execute(command, input):
	p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	last_line = p.communicate(input)[0]

	if p.returncode != 0:
		print "Status error:  ", p.returncode
		
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
	
srcmltranslator = os.environ.get("SRC2SRCML_BIN")
if srcmltranslator == "":
	srcmltranslator = "../../bin/src2srcml"
	
srcmlutility = os.environ.get("SRCML2SRC_BIN")
if srcmlutility == "":
	srcmlutility = "../../bin/src2srcml"

handles_src_encoding = os.environ.get("SRC2SRCML_SRC_ENCODING")

if handles_src_encoding == "":
	default_encoding = "UTF-8"
else:
	default_encoding = "ISO-8859-1"

xml_declaration= '<?xml version="1.0" encoding="' + default_encoding + '" standalone="yes"?>'

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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:srcerr="http://www.sdml.info/srcML/srcerr" language="C++"/>
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
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java"/>
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
srcml = """
<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
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
os.system('echo -e "\na;" > sub/a.cpp')
os.system('echo -e "\nb;" > sub/b.cpp')

check([srcmltranslator, "sub/a.cpp", "sub/b.cpp", "-"], "", nestedfile)

check([srcmltranslator, "--nested", "sub/a.cpp", "-"], "", nestedfile1)

filelist = """
sub/a.cpp
sub/b.cpp
"""

os.system('echo -e "\nsub/a.cpp\nsub/b.cpp\n" > filelistab')

check([srcmltranslator, "--input-file", "filelistab"], "", nestedfile)

####
# srcml2src

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo" version="1.2"/>
"""

checkallforms(srcmlutility, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", srcml, "C++")
checkallforms(srcmlutility, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", srcml, "bar")
checkallforms(srcmlutility, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", srcml, "foo")
checkallforms(srcmlutility, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", srcml, "1.2")
checkallforms(srcmlutility, option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "", srcml, default_encoding)

check([srcmlutility, option.NESTED_FLAG], srcml, "0")
check([srcmlutility, option.NESTED_FLAG], nestedfile, "2")

checkallforms(srcmlutility, "-U", option.UNIT_FLAG, "1", nestedfile, sfile1)
check([srcmlutility, option.UNIT_FLAG, "1", "-"], nestedfile, sfile1)

checkallforms(srcmlutility, "-U", option.UNIT_FLAG, "2", nestedfile, sfile2)
check([srcmlutility, option.UNIT_FLAG, "2"], nestedfile, sfile2)

sxmlfile1 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""
check([srcmlutility, "--xml", option.UNIT_FLAG, "1", "-"], nestedfile, sxmlfile1)
check([srcmlutility, "--xml", option.UNIT_FLAG, "1"], nestedfile, sxmlfile1)

sxmlfile2 = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>
"""
check([srcmlutility, "--xml", option.UNIT_FLAG, "2", "-"], nestedfile, sxmlfile2)
check([srcmlutility, "--xml", option.UNIT_FLAG, "2"], nestedfile, sxmlfile2)

os.system("rm -f sub/a.cpp sub/b.cpp")

check([srcmlutility, "--extract-all"], nestedfile, "")

validate(open("sub/a.cpp", "r").read(), sfile1)
validate(open("sub/b.cpp", "r").read(), sfile2)

##
# src2srcml error return

# invalid input filename
validate(getreturn([srcmltranslator, "foobar"], nestedfile), status.STATUS_INPUTFILE_PROBLEM)

# unknown option
validate(getreturn([srcmltranslator, "--strip"], nestedfile), status.STATUS_UNKNOWN_OPTION)

# unknown encoding

if handles_src_encoding == "":
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_UNKNOWN_ENCODING)
	validate(getreturn([srcmltranslator, option.ENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_UNKNOWN_ENCODING)

# missing value
validate(getreturn([srcmltranslator, option.LANGUAGE_FLAG, bad_language], nestedfile), status.STATUS_INVALID_LANGUAGE)
validate(getreturn([srcmltranslator, option.LANGUAGE_FLAG], nestedfile), 7)
validate(getreturn([srcmltranslator, option.FILENAME_FLAG], nestedfile), 8)
validate(getreturn([srcmltranslator, option.DIRECTORY_FLAG], nestedfile), 9)
validate(getreturn([srcmltranslator, option.SRCVERSION_FLAG], nestedfile), 10)

# source encoding not given
if handles_src_encoding == "":
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG], nestedfile), 11)
validate(getreturn([srcmltranslator, option.ENCODING_FLAG], nestedfile), 12)

##
# srcml2src error return

# invalid input filename
validate(getreturn([srcmlutility, "foobar"], nestedfile), status.STATUS_INPUTFILE_PROBLEM)

# unknown option
validate(getreturn([srcmlutility, "--strip"], nestedfile), status.STATUS_UNKNOWN_OPTION)

# unknown encoding
if handles_src_encoding == "":
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_UNKNOWN_ENCODING)

# source encoding not given
validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG], nestedfile), status.STATUS_SRCENCODING_MISSING)

# unit option selected but no value
validate(getreturn([srcmlutility, option.UNIT_FLAG], nestedfile), status.STATUS_UNIT_MISSING)

# unit value too large
validate(getreturn([srcmlutility, option.UNIT_FLAG, "3"], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, "3", option.XML_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, "3", option.FILENAME_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, "3", option.DIRECTORY_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, "3", option.SRCVERSION_FLAG], nestedfile), status.STATUS_UNIT_INVALID)

exit
