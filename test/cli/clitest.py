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

debug = 0

def check(command, input, output):
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

def checkallforms(base, shortflag, longflag, optionvalue, src, srcml):
	check([base, shortflag, optionvalue], src, srcml)

	check([base, longflag, optionvalue], src, srcml)

	check([base, longflag + "=" + optionvalue], src, srcml)

	return
	
srcmltranslator = os.environ.get("SRC2SRCML_BIN")
if srcmltranslator == "":
	srcmltranslator = "../../bin/src2srcml"
	
srcmlutility = os.environ.get("SRCML2SRC_BIN")
if srcmlutility == "":
	srcmlutility = "../../bin/src2srcml"

handles_src_encoding = os.environ.get("SRC2SRCML_SRC_ENCODING")

srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
"""

checkallforms(srcmltranslator, "-l", "--language", "C++", "", srcml)


srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"/>
"""
check([srcmltranslator, "-l", "C"], "", srcml)

check([srcmltranslator, "--language", "C"], "", srcml)

check([srcmltranslator, "--language=C"], "", srcml)


srcml = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="Java"/>
"""
check([srcmltranslator, "-l", "Java"], "", srcml)

check([srcmltranslator, "--language", "Java"], "", srcml)

check([srcmltranslator, "--language=Java"], "", srcml)

exit
