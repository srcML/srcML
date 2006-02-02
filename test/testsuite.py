#!/usr/bin/python2.4
#
# update.py
#
# Michael L. Collard

import sys
import os.path
import string
import re
import subprocess
import difflib
import pprint

debug = 0

maxcount = 500

entire_file = ""

startcmd = ""

srcmltranslator = os.environ.get("SRC2SRCML_BIN")
if srcmltranslator == "":
	srcmltranslator = "../bin/src2srcml"
	
srcmlutility = os.environ.get("SRCML2SRC_BIN")
if srcmlutility == "":
	srcmlutility = "../bin/src2srcml"

# extracts a particular unit from a srcML file
def extract_unit(src, count):

	last_line=subprocess.Popen([startcmd + srcmlutility, "--xml", "--unit=" + str(count)], stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(src)[0]

	return last_line

def name2file(src_filename):
	file = open(src_filename)
	return file

def name2filestr(src_filename):
	file = open(src_filename).read()
	
	return file

# converts a srcML file back to text
def srcml2src(srctext, encoding):


	# run the srcml processorn
	if srcml2src_src_encoding:

		last_line=subprocess.Popen([startcmd + srcmlutility, "--src-encoding=" + encoding], stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(srctext)[0]

	else:
		last_line=subprocess.Popen([startcmd + srcmlutility], stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(srctext)[0]

	return last_line

# find differences of two files
def xmldiff(xml_filename1, xml_filename2):
			      

	if xml_filename1 != xml_filename2:
		print sys.stdout.writelines(list(difflib.unified_diff(xml_filename1.splitlines(1), xml_filename2.splitlines(1))))
		return 1
	else:

		return 0

# find differences of two files
def src2srcML(text_file, encoding, directory, filename):

	# run the srcml processorn
	if src2srcml_src_encoding:

		last_line = subprocess.Popen([startcmd + srcmltranslator, "-l", ulanguage, "-d", directory, "--src-encoding=" + encoding, "--xml-encoding=" + encoding, "--filename=" + filename], stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(text_file)[0]
	else:
		last_line = subprocess.Popen([startcmd + srcmltranslator, "-l", ulanguage, "-d", directory, "--xml-encoding=" + encoding, "--filename=" + filename], stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(text_file)[0]

	return last_line


#
def getsrcmlattribute(xml_file, command):

	last_line=subprocess.Popen([startcmd + srcmlutility, command], stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(xml_file)[0]
	
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

# version of src2srcml
def src2srcmlversion():

	last_line=subprocess.Popen([startcmd + srcmltranslator, "-V"], stdout=subprocess.PIPE).communicate()[0]

	return string.strip(last_line)

# version of srcml2src
def srcml2srcversion():

	last_line=subprocess.Popen([startcmd + srcmlutility, "-V"], stdout=subprocess.PIPE).communicate()[0]

	return string.strip(last_line)

# number of nested units
def getnested(xml_file):

	last_line=subprocess.Popen([startcmd + srcmlutility, "-n"], stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(xml_file)[0]

	return int(last_line)

src2srcml_src_encoding = src2srcmlversion().find("Libxml2") != -1;
srcml2src_src_encoding = srcml2srcversion().find("Libxml2") != -1;

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
for root, dirs, files in os.walk(source_dir, topdown=True):

	for name in files:

		if os.path.splitext(name)[1] != ".xml":
			continue

		xml_filename = os.path.join(root, name)

		# read file into string
		entire_file = name2filestr(xml_filename)
		
		# extract the filename of the entire document
		ufilename = getdirectory(entire_file)
		
		# only perform for specname if given
		if specname != "" and m.match(ufilename) == None:
			continue
		
		# extract the language of the entire document
		ulanguage = getlanguage(entire_file)
		if len(ulanguage) == 0:
			ulanguage = "C++"

		if speclang != "" and ulanguage != speclang:
			continue
		
		print
		print ulanguage, "\t", ufilename,

		# store the encoding of the outer unit
		encoding = getencoding(entire_file)
		
		# store the directory of the outer unit
		directory = ufilename
		
		# store the version of the outer unit
		version = getversion(entire_file)
		
		# extract the number of units
		number = getnested(entire_file)
		
		if specnum == 0:
			count = 0
		else:
			count = specnum - 1

		while count == 0 or count < number:

			count = count + 1

			if specnum != 0 and count > specnum:
				break

			if count > maxcount:
				break
			
			total_count = total_count + 1
			print count, 

			pfilename = ufilename + str(count)

			# save the particular nested unit
			if number == 0:
				unit_xml_file_sub_str = entire_file
			else:
				unit_xml_file_sub_str = extract_unit(entire_file, count)

			# convert the nested unit to text
			unit_text_file_str = srcml2src(unit_xml_file_sub_str, encoding)

			# convert the text unit to srcML
			unit_srcml_file = src2srcML(unit_text_file_str, encoding, directory, getfilename(unit_xml_file_sub_str))
			# find the difference
			error = xmldiff(unit_xml_file_sub_str, unit_srcml_file)
			error_count += error
			if error == 1:
				errorlist.append((ufilename + " " + ulanguage, count))
				
	break

print
if error_count == 0:
	print "No errors out of " + str(total_count) + " cases" 
else:
	print "Errors:  " + str(error_count) + " out of " + str(total_count) + " cases"
	print "Errorlist:"
	for e in errorlist:
		print e[0], e[1]
print
print src2srcmlversion()
print srcml2srcversion()
print "src2srcml:  ", srcmltranslator
print "srcml2src:  ", srcmlutility

exit
