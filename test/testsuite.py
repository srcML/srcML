#!/usr/bin/python2.4
#
# update.py
#
# Michael L. Collard

import sys
import os.path
import string
import tempfile
import re
import subprocess

debug = 0

maxcount = 40

srcmltranslator = os.environ.get("SRC2SRCML_BIN")
if srcmltranslator == "":
	srcmltranslator = "../bin/src2srcml"
	
srcmlutility = os.environ.get("SRCML2SRC_BIN")
if srcmlutility == "":
	srcmlutility = "../bin/src2srcml"

handles_src_encoding = os.environ.get("SRC2SRCML_SRC_ENCODING")

entire_file = ""

# extracts a particular unit from a srcML file
def extract_unit(src_filename, count):
	filename = tempfile.NamedTemporaryFile()

	subprocess.call([startcmd+srcmlutility, "--xml", "--unit=" + str(count), src_filename, filename.name])

	return filename

def name2file(src_filename):
	file = open(src_filename)
	return file

# converts a srcML file back to text
def srcml2src(xml_filename, encoding):
	filename = tempfile.NamedTemporaryFile()

	# run the srcml processor
	subprocess.call([startcmd+srcmlutility, "--src-encoding=" + encoding, xml_filename, filename.name])
	
	return filename

# find differences of two files
def xmldiff(text_filename, xml_filename1, xml_filename2):

	last_line=subprocess.Popen(["/usr/bin/diff", xml_filename1, xml_filename2], stdout=subprocess.PIPE).communicate()[0]

	trimmed = string.strip(last_line)
	
	if trimmed != "":
		print pfilename
		errors[ufilename] = 1
		os.system("cat " + text_filename)
		print trimmed
		return 1
	else:
		return 0

# find differences of two files
def src2srcML(text_file, encoding, directory, filename):
	pfilename = ""

	# get the text output filename
	xml_file = tempfile.NamedTemporaryFile()

	# run the srcml processorn
	if handles_src_encoding != None:

		subprocess.call([startcmd + srcmltranslator, "-l", ulanguage, "-d", directory, "--src-encoding=" + encoding, "--xml-encoding=" + encoding, "--filename=" + filename, text_file.name, xml_file.name])

	else:
		
		subprocess.call([startcmd + srcmltranslator, "-l", ulanguage, "-d", directory, "--filename=" + filename, text_file.name, xml_file.name])

	return xml_file

# find differences of two files
def getsrcmlattribute(xml_file, command):

	last_line=subprocess.Popen([startcmd + srcmlutility, command, xml_file], stdout=subprocess.PIPE).communicate()[0]
	return string.strip(last_line)

# directory attribute
def getdirectory(xml_file):

	return getsrcmlattribute(xml_file, "-d")

# language attribute
def getlanguage(xml_file):
	
	return getsrcmlattribute(xml_file, "-l")

# xml encoding
def getencoding(xml_file):

	return getsrcmlattribute(xml_file, "-x")

# version attribute
def getversion(xml_file):

	return getsrcmlattribute(xml_file, "-x")

# filename attribute
def getfilename(xml_file):

	return getsrcmlattribute(xml_file, "-f")

# version of the srcML tools
def srcversion():

	last_line=subprocess.Popen([startcmd + srcmlutility, "-V"], stdout=subprocess.PIPE).communicate()[0]

	return string.strip(last_line)

# number of nested units
def getnested(xml_file):

	last_line=subprocess.Popen([startcmd + srcmlutility, "-n", xml_file], stdout=subprocess.PIPE).communicate()[0]

	return int(last_line)

startcmd = ""

specname = ""
if len(sys.argv) > 1:
	specname = sys.argv[1]

print specname

specnum = 0
speclang = ""
if len(sys.argv) == 3:
	result = sys.argv[2]
	if result == "C++" or result == "C" or result == "Java":
		speclang = result
	else:
		specnum = int(sys.argv[2])
elif len(sys.argv) > 2:
	specnum = int(sys.argv[2])

	if len(sys.argv) > 3:
		speclang = sys.argv[3]

# base directory
base_dir = "../suite"

errors = {}
errorlist = []
#if not(os.path.isfile(srcmltranslator)):
#	print srcmltranslator + " does not exist."
#	exit

m = re.compile(specname + "$")

# source directory
source_dir = base_dir
error_count = 0
total_count = 0
for root, dirs, files in os.walk(source_dir):

	save_dirs = dirs
	for name in files:
		froot, fext = os.path.splitext(name)
		xml_filename = os.path.join(root, name)

		if fext != ".xml":
			continue

		# extract the filename of the entire document
		ufilename = getdirectory(xml_filename)
		
		# only perform for specname if given
		if specname != "" and m.match(ufilename) == None:
			continue
		
		# read file into string
		#entire_file = open(xml_filename, 'r').read()
		
		# extract the language of the entire document
		ulanguage = getlanguage(xml_filename)
		if len(ulanguage) == 0:
			ulanguage = "C++"

		if speclang != "" and ulanguage != speclang:
			continue
		
		print
		print ulanguage, "\t", ufilename,

		# store the encoding of the outer unit
		encoding = getencoding(xml_filename)
		
		# store the encoding of the outer unit
		directory = getdirectory(xml_filename)
		
		# store the version of the outer unit
		version = getversion(xml_filename)
		
		# extract the number of units
		number = getnested(xml_filename)

		count = 0
		while count == 0 or count < number:

			count = count + 1

			if specnum != 0 and specnum != count:
				continue

			if count > maxcount:
				break;
			
			total_count = total_count + 1
			print count, 

			pfilename = ufilename + str(count)

			# save the particular nested unit
			if number == 0:
				unit_xml_file_sub = name2file(xml_filename)
			else:
				unit_xml_file_sub = extract_unit(xml_filename, count)

			# convert the nested unit to text
			unit_text_file = srcml2src(unit_xml_file_sub.name, encoding)

			# convert the text unit to srcML
			unit_srcml_file = src2srcML(unit_text_file, encoding, directory, getfilename(unit_xml_file_sub.name))

			# find the difference
			error = xmldiff(unit_text_file.name, unit_xml_file_sub.name, unit_srcml_file.name)
			error_count += error
			if error == 1:
				errorlist.append((ufilename + " " + ulanguage, count))
				

if error_count == 0:
	print
	print srcversion() + ":  No errors out of " + str(total_count) + " cases" 
else:
	print srcversion() + ":  Errors:  " + str(error_count) + " out of " + str(total_count) + " cases"
	print "Errorlist:"
	for e in errorlist:
		print e[0], e[1]
exit
