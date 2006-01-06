#!/usr/bin/python
#
# update.py
#
# Michael L. Collard

import libxml2
import sys
import os.path
import string
import time
import tempfile
import libxslt
import re

debug = 0

maxcount = 40

srcmltranslator = os.environ.get("SRC2SRCML_BIN")

srcmlutility = os.environ.get("SRCML2SRC_BIN")

handles_src_encoding = os.environ.get("SRC2SRCML_SRC_ENCODING")

# extracts a particular unit from a srcML file
def extract_unit(src_filename, count):
	filename = tempfile.NamedTemporaryFile()

	# run the srcml processor
	command = startcmd + srcmlutility + " --xml --unit=" + str(count) + " " + src_filename + " " + filename.name
	if debug:
		print command
	p = os.system(command)
	
	return filename

# extracts a particular unit from a srcML file
def name2file(src_filename):
	file = open(src_filename)
	return file

def xml2txt(xml_filename, encoding):
	filename = tempfile.NamedTemporaryFile()

	# run the srcml processor
	command = startcmd + srcmlutility + " --src-encoding=" + encoding + " " + xml_filename + " " + filename.name
	if debug:
		print command
	p = os.system(command)

	#os.system("cat " + filename.name)
	
	return filename

# extracts a particular unit from a srcML file
def remove_firstline(xml_filename):
	filename = tempfile.NamedTemporaryFile()

	command = "/bin/sed -n '2,$p' " + xml_filename  + " > " + filename.name
	os.system(command)
	if debug:
		print command

	return filename


# find differences of two files
def xmldiff(text_filename, xml_filename1, xml_filename2):

	error_count = 0
	
	# remove the first line from the first file
	xml_file_c1 = remove_firstline(xml_filename1)

	# remove the first two lines from the second file
	xml_file_c2sub = remove_firstline(xml_filename2)
	xml_file_c2 = remove_firstline(xml_file_c2sub.name)
	
	command = "cat " + xml_file_c1.name
#	os.system(command)
	command = "cat " + xml_file_c2.name
#	os.system(command)

	# run diff
	diff_result_file = tempfile.NamedTemporaryFile()
	command = "/usr/bin/diff " + xml_file_c1.name + " " + xml_file_c2.name + " > " + diff_result_file.name
	result = os.system(command)
	if result == 256:
		print pfilename
		error_count = error_count + 1
		errors[ufilename] = count
		os.system("cat " + text_filename)
		os.system("cat " + diff_result_file.name)
	return error_count

# find differences of two files
def src2srcML(text_file, encoding):
	pfilename = ""

	# get the text output filename
	xml_file = tempfile.NamedTemporaryFile()

	# run the srcml processor
	command = startcmd + srcmltranslator + " -l " + ulanguage
	if handles_src_encoding != None:
		command = command + " --src-encoding=" + encoding
		command = command + " --xml-encoding=" + encoding
	command = command + " " + text_file.name + " " + xml_file.name
	if debug:
		print command
	os.system(command)
	return xml_file

# find differences of two files
def srcversion():

	# run the srcml processor
	command = startcmd + srcmltranslator + " -V"
	if debug:
		print command
	p = os.popen(command, 'r')
	return string.strip(p.readline())

# find differences of two files
def getdirname(xml_file):

	# run the srcml processor
	command = startcmd + srcmlutility + " -d " + " " + xml_file
	#if debug:
	#	print command
	p = os.popen(command, 'r')
	name = string.strip(p.readline())
	return name

# find differences of two files
def getlanguage(xml_file):

	# run the srcml processor
	command = startcmd + srcmlutility + " -l " + " " + xml_file
	if debug:
		print command
	p = os.popen(command, 'r')
	return string.strip(p.readline())

# find differences of two files
def getnested(xml_file):

	# run the srcml processor
	command = startcmd + srcmlutility + " -n " + " " + xml_file
	if debug:
		print command
	p = os.popen(command, 'r')
	return int(string.strip(p.readline()))


# find differences of two files
def getencoding(xml_file):

	# run the srcml processor
	command = startcmd + srcmlutility + " -x " + " " + xml_file
	if debug:
		print command
	p = os.popen(command, 'r')
	return string.strip(p.readline())


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
		ufilename = getdirname(xml_filename)
		
		# only perform for specname if given
		if specname != "" and m.match(ufilename) == None:
			continue
		
		# extract the language of the entire document
		res = getlanguage(xml_filename)
		if len(res) > 0:
			ulanguage = res # res[0].content
		else:
			ulanguage = "C++"

		if speclang != "" and ulanguage != speclang:
			continue
		
		print
		print ulanguage, "\t", ufilename,

		# store the encoding
		encoding = getencoding(xml_filename)
		
		# extract the number of units
		number = getnested(xml_filename)
		if number == 0:
			number = 1

		count = 0
		while count < number:

			count = count + 1

			if specnum != 0 and specnum != count:
				continue

			if count > maxcount:
				break;
			
			total_count = total_count + 1
			print count, 

			pfilename = ufilename + str(count)

			# save the particular nested unit
			if number == 1:
				unit_xml_file_sub = name2file(xml_filename)
			else:
				unit_xml_file_sub = extract_unit(xml_filename, count)

			unit_xml_file = remove_firstline(unit_xml_file_sub.name)

			# convert the nested unit to text
			unit_text_file = xml2txt(unit_xml_file_sub.name, encoding)

			# convert the text unit to srcML
			unit_srcml_file = src2srcML(unit_text_file, encoding)

			# find the difference
			error = xmldiff(unit_text_file.name, unit_xml_file.name, unit_srcml_file.name)
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
