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
from xml import xpath
from xml.dom.ext.reader import Sax2
import libxslt

identity_styledoc = libxml2.parseFile("/home/collard/srcML/test/xsl/identity.xsl")
identity_style = libxslt.parseStylesheetDoc(identity_styledoc)

extract_styledoc = libxml2.parseFile("/home/collard/srcML/test/xsl/extractunit.xsl")
extract_style = libxslt.parseStylesheetDoc(extract_styledoc)

#extract_unit_filename = tempfile.NamedTemporaryFile()

#xml2txt_filename = tempfile.NamedTemporaryFile()

#remove_firstline_filename = tempfile.NamedTemporaryFile()

#xmldiff_diff_result_file = tempfile.NamedTemporaryFile()

#src2srcml_xml_file = tempfile.NamedTemporaryFile()

debug = 0

srcmltranslator = " /home/collard/srcML/exp/src2srcml.apr26 "

# extracts a particular unit from a srcML file
def extract_unit(src_filename, count):
	filename = tempfile.NamedTemporaryFile() # extract_unit_filename

	doc = libxml2.parseFile(src_filename)
	result = extract_style.applyStylesheet(doc, { "number" : str(count) })

	extract_style.saveResultToFilename(filename.name, result, 0)

	return filename

# extracts a particular unit from a srcML file
def name2file(src_filename):
	file = open(src_filename)
	return file

# extracts all units from a srcML file
def extract_all_units(doc):
	return xpath.Evaluate('/unit/unit', doc.documentElement)

# extract the filename of the entire document
def extract_filename(doc):
	attr = xpath.Evaluate('/unit', doc.documentElement)[0].attributes
	return attr.get(attr.keys()[1]).nodeValue

# extracts a particular unit from a srcML file
def xml2txt(xml_filename):
	filename = tempfile.NamedTemporaryFile() # xml2txt_filename

	doc = libxml2.parseFile(xml_filename)
	result = identity_style.applyStylesheet(doc, None)

	identity_style.saveResultToFilename(filename.name, result, 0)

	return remove_firstline(filename.name)

# extracts a particular unit from a srcML file
def remove_firstline(xml_filename):
	filename = tempfile.NamedTemporaryFile() # remove_firstline_filename

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

	# remove the first line from the second file
	xml_file_c2 = remove_firstline(xml_filename2)
	
	command = "cat " + xml_file_c1.name
#	os.system(command)
	command = "cat " + xml_file_c2.name
#	os.system(command)

	# run diff
	diff_result_file = tempfile.NamedTemporaryFile() #xmldiff_diff_result_file
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
def src2srcML(text_file):
	pfilename = ""

	# get the text output filename
	xml_file = tempfile.NamedTemporaryFile() #src2srcml_xml_file

	# run the srcml processor
	command = startcmd + srcmltranslator + text_file.name + " " + xml_file.name
	if debug:
		print command
	os.system(command)
	return xml_file

specname = ""
if len(sys.argv) > 1:
	specname = sys.argv[1]

print specname

specnum = 0
if len(sys.argv) > 2:
	specnum = int(sys.argv[2])


startcmd = ""
if len(sys.argv) > 3:
	startcmd = sys.argv[3]

# base directory
base_dir = "/home/collard/srcML/test/suite"

errors = {}

#if not(os.path.isfile(srcmltranslator)):
#	print srcmltranslator + " does not exist."
#	exit
	
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

		# input the xml file
		doc2 = libxml2.parseFile(xml_filename)
		ctxt = doc2.xpathNewContext()

		# extract the filename of the entire document
		res = ctxt.xpathEval("/*/@dir")
		ufilename = res[0].content

		# only perform for specname if given
		if specname != "" and specname != ufilename:
			continue
		
		print
		print ufilename, 

		# extract the number of units
		res = ctxt.xpathEvalExpression("/*/*[1]")
		number = 1
		if res[0].name == "unit":
			number = ctxt.xpathEvalExpression("count(/*/*)")
		
		doc2.freeDoc()
		ctxt.xpathFreeContext()

		count = 0
		while count < number:

			count = count + 1

			if specnum != 0 and specnum != count:
				continue

			if count > 80:
				break;
			
			total_count = total_count + 1
			print count, 

			pfilename = ufilename + str(count)

			# save the particular nested unit
			if number == 1:
				unit_xml_file = name2file(xml_filename)
			else:
				unit_xml_file = extract_unit(xml_filename, count)
			
			# convert the nested unit to text
			unit_text_file = xml2txt(unit_xml_file.name)

			# convert the text unit to srcML
			unit_srcml_file = src2srcML(unit_text_file)

			# find the difference
			error = xmldiff(unit_text_file.name, unit_xml_file.name, unit_srcml_file.name)
			error_count += error

if error_count == 0:
	print
	print "No errors out of " + str(total_count) + " cases" 
else:
	print "Errors:  " + str(error_count) + " out of " + str(total_count) + " cases"
exit
