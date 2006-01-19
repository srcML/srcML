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

# extracts a particular unit from a srcML file
def name2file(src_filename):
	file = open(src_filename)
	return file

def xml2txt(xml_filename, encoding):
	filename = tempfile.NamedTemporaryFile()

	# run the srcml processor
	subprocess.call([startcmd+srcmlutility, "--src-encoding=" + encoding, xml_filename, filename.name])
	
	return filename

# find differences of two files
def xmldiff(text_filename, xml_filename1, xml_filename2):

	error_count = 0
	
	# run diff
	diff_result_file = tempfile.NamedTemporaryFile()
	command = "/usr/bin/diff " + xml_filename1 + " " + xml_filename2 + " > " + diff_result_file.name
	result = os.system(command)
	if result == 256:
		print pfilename
		error_count = error_count + 1
		errors[ufilename] = count
		os.system("cat " + text_filename)
		os.system("cat " + diff_result_file.name)
	return error_count

# find differences of two files
def src2srcML(text_file, encoding, directory, filename):
	pfilename = ""

	# get the text output filename
	xml_file = tempfile.NamedTemporaryFile()

	# run the srcml processorn
	command = startcmd + srcmltranslator + " -l " + ulanguage + " -d " + directory + " --filename=\"" + filename + "\""
	if handles_src_encoding != None:
		command = command + " --src-encoding=" + encoding
		command = command + " --xml-encoding=" + encoding
	command = command + " " + text_file.name + " " + xml_file.name
	if debug:
		print command

	p = subprocess.Popen(command, shell=True)
	os.waitpid(p.pid, 0)

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
def getdirectory(xml_file):

	last_line=subprocess.Popen([startcmd + srcmlutility, "-d", xml_file], stdout=subprocess.PIPE).communicate()[0]
	return string.strip(last_line)

# find differences of two files
def getlanguage(xml_file):
	
	last_line=subprocess.Popen([startcmd + srcmlutility, "-l", xml_file], stdout=subprocess.PIPE).communicate()[0]
	return string.strip(last_line)

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

	last_line=subprocess.Popen([startcmd + srcmlutility, "-x", xml_file], stdout=subprocess.PIPE).communicate()[0]
	return string.strip(last_line)

# find differences of two files
def getversion(xml_file):

	last_line=subprocess.Popen([startcmd + srcmlutility, "-s", xml_file], stdout=subprocess.PIPE).communicate()[0]
	return string.strip(last_line)

# find differences of two files
def getfilename(xml_file):

	last_line=subprocess.Popen([startcmd + srcmlutility, "-f", xml_file], stdout=subprocess.PIPE).communicate()[0]

	return string.strip(last_line)

# find differences of two files
def getfilename_unit(xml_file, num):

	# run the srcml processor
	command = startcmd + srcmlutility + " --unit=" + str(num) + " --filename " + " " + xml_file
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
		ufilename = getdirectory(xml_filename)
		
		# only perform for specname if given
		if specname != "" and m.match(ufilename) == None:
			continue
		
		# read file into string
		entire_file = open(xml_filename, 'r').read()
		
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

		# store the encoding of the outer unit
		encoding = getencoding(xml_filename)
		
		# store the encoding of the outer unit
		directory = getdirectory(xml_filename)
		
		# store the version of the outer unit
		version = getversion(xml_filename)
		
		# extract the number of units
		number = getnested(xml_filename)
		#if number == 0:
		#	number = 1

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
			unit_text_file = xml2txt(unit_xml_file_sub.name, encoding)

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
