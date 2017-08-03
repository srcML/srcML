#!/usr/bin/env python

##
# @file testsuite.py
#
# @copyright Copyright (C) 2006-2014 srcML, LLC. (www.srcML.org)
#
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import sys
import os.path
import re
import subprocess
import difflib
import string
from datetime import datetime, time
import fileinput

maxcount = 700
error_filename = "srcMLTestReport"
error_filename_extension = ".txt"

FIELD_WIDTH_LANGUAGE = 15
FIELD_WIDTH_URL = 30
MAX_COUNT = 29
sperrorlist = []
xml_filename = ""

srcmltranslator = os.environ.get("SRC2SRCML")
if srcmltranslator == "" or srcmltranslator == None:
    srcmltranslator = "../bin/src2srcml"

srcmlutility = os.environ.get("SRCML2SRC")
if srcmlutility == "" or srcmlutility == None:
    srcmlutility = "../bin/srcml2src"

# Walk into directories in filesystem
# Ripped from os module and slightly modified
# for alphabetical sorting
#
def sortedWalk(top, topdown=True, onerror=None):
    from os.path import join, isdir, islink

    names = os.listdir(top)
    names.sort()
    dirs, nondirs = [], []

    for name in names:
        if isdir(os.path.join(top, name)):
            dirs.append(name)
        else:
            nondirs.append(name)

    if topdown:
        yield top, dirs, nondirs
    for name in dirs:
        path = join(top, name)
        if not os.path.islink(path):
            for x in sortedWalk(path, topdown, onerror):
                yield x
    if not topdown:
        yield top, dirs, nondirs


# extracts a particular unit from a srcML file
def safe_communicate(command, inp):

    try:
        return subprocess.Popen(command, stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(inp)[0]
    except OSError, (errornum, strerror):
        try:
            return subprocess.Popen(command, stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate(inp)[0]
        except OSError, (errornum, strerror):
            sperrorlist.append((command, xml_filename, errornum, strerror))
            raise

# extracts a particular unit from a srcML file
def safe_communicate_file(command, filename):

    newcommand = command[:]
    newcommand.append(filename)
    try:
        return subprocess.Popen(newcommand, stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate()[0]
    except OSError, (errornum, strerror):
        try:
            return subprocess.Popen(newcommand, stdout=subprocess.PIPE, stdin=subprocess.PIPE).communicate()[0]
        except OSError, (errornum, strerror):
            sperrorlist.append((command, xml_filename, errornum, strerror))
            raise

# extracts a particular unit from a srcML file
def extract_unit(src, count):

    command = [srcmlutility, "--unit=" + str(count), "--xml"]

    return safe_communicate(command, src)

# extracts a particular unit from a srcML file
def extract_all_executable(src):

    command = [srcmlutility, "-0", "--xml"]

    return safe_communicate(command, src)

# extracts a particular unit from a srcML file
def extract_all(src, encoding):

    all = []
    src_all = []

    archive = srcml_archive()
    if src.find("problem") != -1 :
        archive.set_xml_encoding("ISO-8859-1")

    if src.find("unicode") != -1 :
        archive.set_src_encoding("UTF-8")

    archive.read_open_memory(src)

    unit = archive.read_unit_header()
    while unit != None :
        srcml = unit.get_xml_fragment()
        if archive.get_url() != None and (archive.get_url().find(".all") != -1 or archive.get_url().find("unicode") != -1):
            srcml = "<unit>" + srcml[srcml.find(">") + 1:]
        all.append(srcml)
        unit.unparse_memory()
        src_all.append(unit.src())
        unit = archive.read_unit_header()

    archive.close()

    all.append(0)
    src_all.append(0)

    return all, src_all, archive

def name2filestr(src_filename):
    file = open(src_filename).read()

    return file

# converts a srcML file back to text
def srcml2src_executable(srctext, encoding):

    # run the srcml processor
    command = [srcmlutility]
    command.append("--src-encoding=" + encoding)

    return safe_communicate(command, srctext)

# converts a srcML file back to text
def srcml2src(srctext, encoding):

    # run the srcml processor
    utility = srcMLUtility(srctext, len(srctext) + 1, encoding, 0, "")
    source = utility.extract_text(0)
    utility.delete()

    return source

# converts from unix to dos line endings
def unix2dos(srctext):

    # run the srcml processor
    command = ['unix2dos']

    return safe_communicate(command, srctext)

# find differences of two files
def xmldiff(xml_filename1, xml_filename2):

    if xml_filename1 != xml_filename2:
        return list(difflib.unified_diff(xml_filename1.splitlines(1), xml_filename2.splitlines(1)))
    else:
        return ""

# find differences of two files
def src2srcML_executable(text_file, encoding, language, url, filename, prefixlist):

    command = [srcmltranslator, "-l", language, "--encoding=" + encoding]

    if url != "":
        command.extend(["--url", url])

    if filename != "":
        command.extend(["--filename", filename])

    command.extend(prefixlist)

    #print command

    # run the srcml processor
    command.append("--src-encoding=" + encoding)

    command.append("--quiet")

    return safe_communicate(command, text_file)

# find differences of two files
def src2srcML(text_file, encoding, language, url, filename, read_archive):

    if filename == "" :
        filename = None;

    archive = read_archive.clone()
    if url.find("problem") != -1 :
        archive.set_xml_encoding("ISO-8859-1")

    archive.write_open_memory()

    unit = srcml_unit(archive)
    unit.set_language(language)
    if url.find("unicode") != -1 :
        unit.set_src_encoding("UTF-8")

    unit.parse_memory(text_file)
    srcml = unit.get_xml_fragment()
    archive.close()

    srcml = "<unit>" + srcml[srcml.find(">") + 1:]

    return srcml


def getsrcmlattribute(xml_file, command):

    last_line = safe_communicate([srcmlutility, command], xml_file)

    return last_line.strip()

def getsrcmlattributefile(xml_file, command):

    last_line = safe_communicate_file([srcmlutility, command], xml_file)

    return last_line.strip()

def getsrcmlattributeraw(srctext, command):

    # run the srcml processor
    command = [srcmlutility]
    command.append("--info")

    return safe_communicate(command, srctext)

# url attribute
def geturl(xml_file):

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

# xmlns attribute
def getfullxmlns_executable(xml_file):

    l = []
    for a in getsrcmlattribute(xml_file, "--info").split():
        if a[0:5] == "xmlns":
            l.append("--" + a.replace('"', ""))

    return l

# xmlns attribute
def getfullxmlns(xml_file):

    archive = srcml_archive()
    archive.read_open_memory(xml_file)
    l = archive.get_options()

    archive.close()

    return l

# xmlns attribute
def defaultxmlns(l):

    newl = []
    for a in l:
        url = a.split('=')[1]
        if url == 'http://www.sdml.info/srcML/src' or url == 'http://www.sdml.info/srcML/cpp' or url == 'http://www.sdml.info/srcML/srcerr':
            newl.append(a)
    return newl

def nondefaultxmlns(l):

    newl = []
    for a in l:
        url = a.split('=')[1]
        if not(url == 'http://www.sdml.info/srcML/src' or url == 'http://www.sdml.info/srcML/cpp' or url == 'http://www.sdml.info/srcML/srcerr'):
            newl.append(a)
    return newl

# version of src2srcml
def src2srcmlversion():
    last_line = safe_communicate([srcmltranslator, "-V"], "")

    return last_line.splitlines()[0].strip()

# version of srcml2src
def srcml2srcversion():

    last_line = safe_communicate([srcmlutility, "-V"], "")

    return last_line.splitlines()[0].strip()

# number of nested units
def getnested(xml_file):

    snumber = safe_communicate([srcmlutility, "-n"], xml_file)

    if snumber != "":
        return int(snumber)
    else:
        return 0

class Tee(object):
    def __init__(self, name):
        self.file = open(name, "w")
        self.stdout = sys.stdout
        sys.stdout = self

    def __del__(self):
        sys.stdout = self.stdout
        self.file.close()

    def write(self, data):
        self.file.write(data)
        self.stdout.write(data)

Tee(error_filename)

print "Testing:"
print


# Handle optional dos line endings
doseol = False
use_exec = False
while len(sys.argv) > 1 and ( sys.argv[1] == "--dos" or sys.argv[1] == "--exec" ) :
    if sys.argv[1] == "--dos" :
        doseol = True
    else :
        use_exec = True
    sys.argv.pop(0)

if use_exec :
    print src2srcmlversion()
    print srcml2srcversion()
else :
    sys.path.append("../python/src")
    from srcml import *
    print version_string()

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
    if result == "C++" or result == "C" or result == "C#" or result == "Objective-C" or result == "Java" or result == "C++11" or result == "AspectJ":
        speclang = result
    else:
        specnum = int(sys.argv[2])
elif len(sys.argv) == 2:
    result = sys.argv[1]
    if result == "C++" or result == "C" or result == "C#" or result == "Objective-C" or result == "Java" or result == "C++11" or result == "AspectJ":
        speclang = result
        specname = ""
    else:
        specname = result
elif len(sys.argv) > 2:
    specnum = int(sys.argv[2])

    if len(sys.argv) > 3:
        speclang = sys.argv[3]

# base url
base_dir = "suite"

errorlist = []

#if not(os.path.isfile(srcmltranslator)):
#       print srcmltranslator + " does not exist."
#       exit

m = re.compile(specname + "$")

# source url
source_dir = base_dir

# total number of errors
error_count = 0

# total test cases
total_count = 0

root = "suite"

try:
    # process all files
    flag = True
    print "HERE"
    while flag:

        print "A1"
        inparser = open("inparser")
    	file = inparser.readline()
#    	if len(file) == 0:
#    		print "DONE"
#    		break
        print "C"
    	files = [file]

        # process all files
        for name in files:
            print "B"
            try:
                # only process xml files
                if os.path.splitext(name)[1] != ".xml":
                    continue

                # full path of the file
                xml_filename = os.path.join(root, name)

                f = open(xml_filename, "r")

                line1 = f.readline()
                filexml = line1
                s = f.readline()
                filexml += s

                language = s.split("language=\"")[1].split('"')[0]

                part = s.split("url=\"")
                url = part[1].split('"')[0] if len(part) > 1 else None

                part = s.split("filename=\"")
                filename = part[1].split('"')[0] if len(part) > 1 else None

                # only process if url name matches or is not given
                if specname != "" and m.match(url) == None and (filename == None or m.match(filename) == None):
                    f.close()
                    continue

                # only process if language matches or is not given
                if speclang != "" and language != speclang:
                    f.close()
                    continue

                filexml += f.read()
                f.close()

                # output language and url
                print
                print language.ljust(FIELD_WIDTH_LANGUAGE), " ", url.ljust(FIELD_WIDTH_URL), " ",

                # encoding of the outer unit
                part = line1.split("encoding=\"")
                encoding = part[1].split('"')[0]

                if specnum == 0:
                    count = 0
                else:
                    count = specnum - 1

                if use_exec :
                    all = string.split(extract_all_executable(filexml), '\0')
                else :
                    all, src_all, read_archive = extract_all(filexml, encoding)

                number = len(all) - 1
                if use_exec :
                    fxmlns = defaultxmlns(getfullxmlns_executable(filexml))

                if use_exec :
                    xmlns = defaultxmlns(getfullxmlns_executable(filexml))
                    while len(xmlns) == 0 :
                        xmlns = defaultxmlns(getfullxmlns_executable(filexml))

                line_count = 0
                print "COUNT IS ", count
                while count == 0 or count < number:
                    print "COUNT IS ", count
                    try:

                        count = count + 1

                        if specnum!= 0 and count > specnum:
                            break

                        if count > maxcount:
                            break

                        #print all[count]

                        # total count of test cases
                        total_count = total_count + 1

                        # save the particular nested unit
                        if number == 0:
                            unitxml = filexml
                        else:
                            unitxml = all[count - 1]

                        # convert the unit in xml to text
                        if use_exec :
                            unittext = srcml2src_executable(unitxml, encoding)
                        else :
                            unittext = src_all[count - 1]

                        # convert the unit in xml to text (if needed)
                        if doseol:
                            unittext = unix2dos(unittext)

                        # convert the text to srcML
                        if use_exec :
                            unitsrcmlraw = src2srcML_executable(unittext, encoding, language, url, getfilename(unitxml), xmlns)
                        else :
                            unitsrcmlraw = src2srcML(unittext, encoding, language, url, filename, read_archive)

                        # additional, later stage processing
                        unitsrcml = unitsrcmlraw

                        # find the difference
                        result = xmldiff(unitxml, unitsrcml)

                        if count > 0 and line_count >= 75:
                            print "\n", "".rjust(FIELD_WIDTH_LANGUAGE), " ", "...".ljust(FIELD_WIDTH_URL), " ",
                            line_count = 0

                        if count > 99 :
                            line_count += 3
                        elif count > 9 :
                            line_count += 2
                        else :
                            line_count += 1

                        line_count += 1

                        if result != "":
                            error_count += 1

                            errorlist.append((url + " " + language, count, result, name))

                            # part of list of nested unit number in output
                            print "\033[0;31m" + str(count) + "\033[0m",
                        elif number != 0:
                            # part of list of nested unit number in output
                            print "\033[0;33m" + str(count) + "\033[0m",

                    except OSError, (errornum, strerror):
                        continue

                    print "EOL"

            except OSError, (errornum, strerror):
                continue

        ki = False
except KeyboardInterrupt:
    ki = True

print "END"

exit
