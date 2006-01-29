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
import pprint

import option

debug = 0

def check(command, input, output):

	print ' '.join(command)
	
	line = execute(command, input)

	validate(string.strip(output), string.strip(line))
	
def validate(org, gen):
	if org != gen:
		print "ERROR"
		print "|" + org + "|"
		print "|" + gen + "|"
	return

def execute(incommand, input):
	last_line=subprocess.Popen(incommand, stdin=subprocess.PIPE, stdout=subprocess.PIPE).communicate(input)[0]

	return last_line

def checkallforms(base, shortflag, longflag, optionvalue, progin, progout):
	check([base, shortflag, optionvalue], progin, progout)

	check([base, longflag, optionvalue], progin, progout)

	check([base, longflag + "=" + optionvalue], progin, progout)

	return
	
srcmltranslator = os.environ.get("SRC2SRCML_BIN")
if srcmltranslator == "":
	srcmltranslator = "../../bin/src2srcml"
	
srcmlutility = os.environ.get("SRCML2SRC_BIN")
if srcmlutility == "":
	srcmlutility = "../../bin/src2srcml"

handles_src_encoding = os.environ.get("SRC2SRCML_SRC_ENCODING")

##
# empty default
srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
check([srcmltranslator], "", srcml)

##
# empty with debug
srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns:srcerr="http://www.sdml.info/srcML/srcerr" language="C++"/>
"""
check([srcmltranslator, "--debug"], "", srcml)

##
# language flag
srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""
checkallforms(srcmltranslator, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C++", "", srcml)

srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
"""
checkallforms(srcmltranslator, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "C", "", srcml)

srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java"/>
"""
checkallforms(srcmltranslator, option.LANGUAGE_FLAG_SHORT, option.LANGUAGE_FLAG, "Java", "", srcml)

##
# filename flag
srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo"/>
"""
checkallforms(srcmltranslator, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "foo", "", srcml)

# filenames are not expanded if specified (unlike when extracted from name)
srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="bar/foo"/>
"""
checkallforms(srcmltranslator, option.FILENAME_FLAG_SHORT, option.FILENAME_FLAG, "bar/foo", "", srcml)

# filename and directory specified
srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo"/>
"""
check([srcmltranslator, option.FILENAME_FLAG_SHORT, "foo", option.DIRECTORY_FLAG_SHORT, "bar"], "", srcml)

srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo"/>
"""
check([srcmltranslator, option.DIRECTORY_FLAG_SHORT, "bar", option.FILENAME_FLAG_SHORT, "foo"], "", srcml)

##
# directory flag
srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar"/>
"""
checkallforms(srcmltranslator, option.DIRECTORY_FLAG_SHORT, option.DIRECTORY_FLAG, "bar", "", srcml)

##
# version flag
srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
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

sxmlfile1 = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""
check([srcmltranslator], sfile1, sxmlfile1)

sfile2 = """
b;
"""

sxmlfile2 = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>
"""
check([srcmltranslator], sfile2, sxmlfile2)

check([srcmltranslator, "-", "sub/a.cpp.xml"], sfile1, "")

check([srcmltranslator, "-", "sub/b.cpp.xml"], sfile2, "")

nestedfile1 = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">

<unit language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

nestedfile = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">

<unit language="C++" dir="sub" filename="a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit language="C++" dir="sub" filename="b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""
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

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" dir="bar" filename="foo" version="1.2"/>
"""

check([srcmlutility, "--language"], srcml, "C++")
check([srcmlutility, "--directory"], srcml, "bar")
check([srcmlutility, "--filename"], srcml, "foo")
check([srcmlutility, "--src-version"], srcml, "1.2")
check([srcmlutility, "--xml-encoding"], srcml, "UTF-8")

check([srcmlutility, "--nested"], srcml, "0")
check([srcmlutility, "--nested"], nestedfile, "2")


exit
