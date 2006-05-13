#!/usr/bin/python2.4
#
# update.py
#
# Michael L. Collard

import sys
import os.path
import re
import subprocess
import difflib

maxcount = 500
error_filename = ".suitelist"

srcmltranslator = os.environ.get("SRC2SRCML")
if srcmltranslator == "":
	srcmltranslator = "../bin/src2srcml"
	
srcmlutility = os.environ.get("SRCML2SRC")
if srcmlutility == "":
	srcmlutility = "../bin/src2srcml"

# extracts a particular unit from a srcML file
def extract_unit(src, count):

	command = [srcmlutility, "--unit=" + str(count), "--xml"]

	return subprocess.Popen(command, stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(src)[0]

def name2filestr(src_filename):
	file = open(src_filename).read()
	
	return file

# converts a srcML file back to text
def srcml2src(srctext, encoding):

	# run the srcml processor
	command = [srcmlutility]
	if srcml2src_src_encoding:
		command.append("--src-encoding=" + encoding)

	return subprocess.Popen(command, stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(srctext)[0]

# find differences of two files
def xmldiff(xml_filename1, xml_filename2):

	if xml_filename1 != xml_filename2:
		print sys.stdout.writelines(list(
			difflib.unified_diff(xml_filename1.splitlines(1), xml_filename2.splitlines(1))))
		return 1
	else:

		return 0

# find differences of two files
def src2srcML(text_file, encoding, language, directory, filename):

	command = [srcmltranslator, "-l", language, "-d", directory,
			   "--xml-encoding=" + encoding, "--filename=" + filename]

	# run the srcml processorn
	if src2srcml_src_encoding:
		command.append("--src-encoding=" + encoding)

	return subprocess.Popen(command, stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(text_file)[0]

#
def getsrcmlattribute(xml_file, command):

	last_line = subprocess.Popen([srcmlutility, command], bufsize=-1,
				     stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(xml_file)[0]
	
	return last_line.strip()

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

	last_line = subprocess.Popen([srcmltranslator, "-V"],
				     stdout=subprocess.PIPE).communicate()[0]

	return last_line.splitlines()[0].strip()

# version of srcml2src
def srcml2srcversion():

	last_line = subprocess.Popen([srcmlutility, "-V"],
				     stdout=subprocess.PIPE).communicate()[0]

	return last_line.splitlines()[0].strip()

# number of nested units
def getnested(xml_file):

	last_line = subprocess.Popen([srcmlutility, "-n"],
				     stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(xml_file)[0]

	return int(last_line)

src2srcml_src_encoding = src2srcmlversion().find("Libxml2") != -1;
srcml2src_src_encoding = srcml2srcversion().find("Libxml2") != -1;

print "Testing:"
print 

print src2srcmlversion()
print srcml2srcversion()
print

specname = ""
if len(sys.argv) > 1:
	specname = sys.argv[1]

if specname != "":
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

# total number of errors
error_count = 0

# total test cases
total_count = 0

try:
			
	# process all files		
	for root, dirs, files in os.walk(source_dir, topdown=True):		

		# process all files
		for name in files:

			# only process xml files
			if os.path.splitext(name)[1] != ".xml":
				continue

			# full path of the file
			xml_filename = os.path.join(root, name)
			
			# read entire file into a string
			filexml = name2filestr(xml_filename)

			# directory of the outer unit element
			directory = getdirectory(filexml)
			
			# only process if directory name matches or is not given
			if specname != "" and m.match(directory) == None:
				continue
			
			# language of the entire document with a default of C++
			language = getlanguage(filexml)
			if len(language) == 0:
				language = "C++"

			# only process if language matches or is not given
			if speclang != "" and language != speclang:
				continue
			
			# output language and directory
			print
			print language, "\t", directory,

			# encoding of the outer unit
			encoding = getencoding(filexml)
			
			# version of the outer unit
			version = getversion(filexml)
		
			# number of nested units
			number = getnested(filexml)
		
			if specnum == 0:
				count = 0
			else:
				count = specnum - 1

			while count == 0 or count < number:

				count = count + 1

				if specnum!= 0 and count > specnum:
					break

				if count > maxcount:
					break

				# total count of test cases
				total_count = total_count + 1

				# part of list of nested unit number in output
				print count, 

				# save the particular nested unit
				if number == 0:
					unitxml = filexml
				else:
					unitxml = extract_unit(filexml, count)

				# convert the unit in xml to text
				unittext = srcml2src(unitxml, encoding)

				# convert the text to srcML
				unitsrcml = src2srcML(unittext, encoding, language, directory, getfilename(unitxml))
			
				# find the difference
				error = xmldiff(unitxml, unitsrcml)
				error_count += error
				if error == 1:
					errorlist.append((directory + " " + language, count))
	ki = False
except KeyboardInterrupt:
	ki = True
	
print
print
print "Report:"
if ki:
	print
	print "Testing stopped by keyboard"
	
# output error counts
print
f = open(error_filename, "w")
if error_count == 0:
	print "No errors out of " + str(total_count) + " cases" 
else:
	print "Errors:  " + str(error_count) + " out of " + str(total_count) + " cases"
	print "Errorlist:"
	for e in errorlist:
		f.write(str(e[0]) + " " + str(e[1]) + "\n")
		print e[0], e[1]
f.close()


# output tool version
print
print src2srcmlversion(), srcmltranslator
print srcml2srcversion(), srcmlutility

exit
