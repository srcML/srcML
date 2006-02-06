#!/usr/bin/python2.4
#
# clitest.py
#
# Michael L. Collard
# collard@cs.kent.edu

import os.path
import subprocess

import option
import status

def check(command, input, output):

	print os.path.basename(command[0]), ' '.join(command[1:])
	
	line = execute(command, input)

	return validate(output.strip(), line.strip())
	
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
if srcmltranslator == "":
	srcmltranslator = "../../bin/src2srcml"
	
srcmlutility = os.environ.get("SRCML2SRC")
if srcmlutility == "":
	srcmlutility = "../../bin/src2srcml"

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

check([srcmltranslator, option.NESTED_FLAG, "sub/a.cpp", "-"], "", nestedfile1)

filelist = """
sub/a.cpp
sub/b.cpp
"""

os.system('echo -e "\nsub/a.cpp\nsub/b.cpp\n" > filelistab')

check([srcmltranslator, option.FILELIST_FLAG, "filelistab"], "", nestedfile)


####
# srcml2src

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo" version="1.2"/>
"""

checkallforms(srcmlutility, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "", srcml, "C++")
checkallforms(srcmlutility, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "", srcml, "bar")
checkallforms(srcmlutility, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "", srcml, "foo")
checkallforms(srcmlutility, option.SRCVERSION_FLAG_SHORT, option.SRCVERSION_FLAG, "", srcml, "1.2")
checkallforms(srcmlutility, option.ENCODING_FLAG_SHORT, option.ENCODING_FLAG, "", srcml, default_srcml2src_encoding)

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
check([srcmlutility, option.EXPAND_FLAG_SHORT], nestedfile, "")

validate(open("sub/a.cpp", "r").read(), sfile1)
validate(open("sub/b.cpp", "r").read(), sfile2)

##
# src2srcml error return

# invalid input filename
validate(getreturn([srcmltranslator, "foobar"], nestedfile), status.STATUS_INPUTFILE_PROBLEM)

# unknown option
validate(getreturn([srcmltranslator, "--strip"], nestedfile), status.STATUS_UNKNOWN_OPTION)

# unknown encoding

if src2srcml_src_encoding:
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_UNKNOWN_ENCODING)
	validate(getreturn([srcmltranslator, option.ENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_UNKNOWN_ENCODING)
else:
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_LIBXML2_FEATURE)
	validate(getreturn([srcmltranslator, option.ENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_SUCCESS)
	
# missing value
validate(getreturn([srcmltranslator, option.LANGUAGE_FLAG, bad_language], nestedfile), status.STATUS_INVALID_LANGUAGE)
validate(getreturn([srcmltranslator, option.LANGUAGE_FLAG], nestedfile), status.STATUS_LANGUAGE_MISSING)
validate(getreturn([srcmltranslator, option.FILENAME_FLAG], nestedfile), status.STATUS_FILENAME_MISSING)
validate(getreturn([srcmltranslator, option.DIRECTORY_FLAG], nestedfile), status.STATUS_DIRECTORY_MISSING)
validate(getreturn([srcmltranslator, option.SRCVERSION_FLAG], nestedfile), status.STATUS_VERSION_MISSING)

# source encoding not given
if src2srcml_src_encoding:
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG], nestedfile), status.STATUS_SRCENCODING_MISSING)
else:
	validate(getreturn([srcmltranslator, option.TEXTENCODING_FLAG], nestedfile), status.STATUS_LIBXML2_FEATURE)

validate(getreturn([srcmltranslator, option.ENCODING_FLAG], nestedfile), status.STATUS_XMLENCODING_MISSING)

##
# srcml2src error return

# invalid input filename
validate(getreturn([srcmlutility, "foobar"], nestedfile), status.STATUS_INPUTFILE_PROBLEM)

# unknown option
validate(getreturn([srcmlutility, "--strip"], nestedfile), status.STATUS_UNKNOWN_OPTION)

# unknown encoding
if srcml2src_src_encoding:
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_UNKNOWN_ENCODING)
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG], nestedfile), status.STATUS_SRCENCODING_MISSING)
else:
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG + "=" + bad_encoding], nestedfile), status.STATUS_LIBXML2_FEATURE)
	validate(getreturn([srcmlutility, option.TEXTENCODING_FLAG], nestedfile), status.STATUS_LIBXML2_FEATURE)
	
# source encoding not given

# unit option selected but no value
validate(getreturn([srcmlutility, option.UNIT_FLAG], nestedfile), status.STATUS_UNIT_MISSING)

# unit value too large
missing_unit = "3";
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit, option.XML_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit, option.FILENAME_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit, option.DIRECTORY_FLAG], nestedfile), status.STATUS_UNIT_INVALID)
validate(getreturn([srcmlutility, option.UNIT_FLAG, missing_unit, option.SRCVERSION_FLAG], nestedfile), status.STATUS_UNIT_INVALID)

print src2srcmlversion()
print srcml2srcversion()

exit
